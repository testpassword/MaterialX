/*
 TM & (c) 2021 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
 All rights reserved.

 Improved and extended by Luxoft 2021
*/

import * as THREE from 'three'
import { GLTFLoader } from 'three/examples/jsm/loaders/GLTFLoader'
import { DRACOLoader } from 'three/examples/jsm/loaders/DRACOLoader'
import { RGBELoader } from 'three/examples/jsm/loaders/RGBELoader.js'
import { OrbitControls } from 'three/examples/jsm/controls/OrbitControls.js'
import { EffectComposer } from 'three/examples/jsm/postprocessing/EffectComposer.js'
import { RenderPass } from 'three/examples/jsm/postprocessing/RenderPass.js'
import { ShaderPass } from 'three/examples/jsm/postprocessing/ShaderPass.js'
import { GammaCorrectionShader } from 'three/examples/jsm/shaders/GammaCorrectionShader.js'
import { prepareEnvTexture, findLights, registerLights, getUniformValues, getMaterialVersion } from './mtlx-threejs_converter.js'
import { FallbackMaterial } from "./FallbackMaterial.js"
import { ColorGUIController } from "./ColorGUIController"
import * as dat from 'dat.gui'

const gui = new dat.GUI({ closed: true, name: 'Property Editor', hideable: false })
let camera, scene, renderer, composer, controls, model, mx
const materialShader = {
  uniforms: undefined,
  lights: undefined,
  lightData: undefined,
  radianceTexture: undefined,
  isTransparent: undefined,
  shader: undefined,
  obj: undefined
}

const getQueryParam = param => new URLSearchParams(document.location.search).get(param)
const getMaterialPath = () => getQueryParam('material') ?? 'public/materials/demo/demo.mtlx'
const getEnvironmentPath = () => getQueryParam('environment') ?? 'public/environments/royal_esplanade.hdr'
const getMeshPath = () => getQueryParam('mesh') ?? 'public/meshes/sphere.glb'

const setQueryParams = () => {
  const meshesSelect = document.getElementById('meshes')
  meshesSelect.value = getMeshPath()
  const environmentsSelect = document.getElementById('environments')
  environmentsSelect.value = getEnvironmentPath()
  meshesSelect.addEventListener('change', e => {
    window.location.href = `${window.location.origin}${window.location.pathname}?material=${getMaterialPath()}&mesh=${e.target.value}&environment=${getEnvironmentPath()}`
  })
  environmentsSelect.addEventListener('change', e => {
    window.location.href = `${window.location.origin}${window.location.pathname}?material=${getMaterialPath()}&mesh=${getMeshPath()}&environment=${e.target.value}`
  })
}


const buildScene = canvasId => {
  const canvas = document.getElementById(canvasId)
  camera = new THREE.PerspectiveCamera(45, window.innerWidth / window.innerHeight, 1, 100)
  scene = new THREE.Scene()
  renderer = new THREE.WebGLRenderer({ canvas, context: canvas.getContext('webgl2'), antialias: true, alpha: true })
  renderer.setClearColor( 0xffffff, 0)
  renderer.setSize(window.innerWidth, window.innerHeight)
  composer = new EffectComposer(renderer)
  composer.addPass(new RenderPass(scene, camera))
  composer.addPass(new ShaderPass( GammaCorrectionShader ))
  window.addEventListener('resize', onWindowResize)
  controls = new OrbitControls(camera, renderer.domElement)
}

/**
 * Load material by query param 'material' or load FallbackMaterial if it didn't exists or bad url.
 */
const catchingLoadMaterial = async (loader, materialPath) => {
  let mtlxMaterial
  mtlxMaterial = await new Promise(resolve => {
    loader.load(
      materialPath,
      resolve,
      undefined,
      () => {
        mtlxMaterial = undefined
        resolve()
      })
  })
  return mtlxMaterial
}

const buildPropertyEditor = () => {
  const zeroToOneParam = (folder, name) => {
    const p = materialShader.uniforms[name]
    if (p) folder.add(p, 'value', 0, 1).step(0.0001).name(name)
  }
  const colorParam = (folder, name) => {
    const p = materialShader.uniforms[name]
    if (p) folder.addColor(new ColorGUIController(materialShader.uniforms, name), 'value').name(name)
  }
  const numericParam = (folder, name, from, to, step) => {
    const p = materialShader.uniforms[name]
    if (p) folder.add(materialShader.uniforms[name], 'value', from, to).step(step).name(name)
  }
  const base = gui.addFolder('Base')
  zeroToOneParam(base, 'base')
  zeroToOneParam(base, 'diffuse_roughness')
  const coat = gui.addFolder('Coat')
  zeroToOneParam(coat, 'coat')
  colorParam(coat, 'coat_color')
  zeroToOneParam(coat, 'coat_roughness')
  zeroToOneParam(coat, 'coat_anisotropy')
  zeroToOneParam(coat, 'coat_rotation')
  zeroToOneParam(coat, 'coat_affect_color')
  zeroToOneParam(coat, 'coat_affect_roughness')
  coat.add(materialShader.uniforms['coat_IOR'], 'value', 0, 3).step(0.0001).name('coat_IOR')
  const emission = gui.addFolder('Emission')
  zeroToOneParam(emission, 'emission')
  colorParam(emission, 'emission_color')
  const geometry = gui.addFolder('Geometry')
  geometry.add(materialShader.uniforms['thin_walled'], 'value').name('thin_walled')
  const sheen = gui.addFolder('Sheen')
  zeroToOneParam(sheen, 'sheen')
  colorParam(sheen, 'sheen_color')
  zeroToOneParam(sheen, 'sheen_roughness')
  const specular = gui.addFolder('Specular')
  zeroToOneParam(specular, 'specular')
  colorParam(specular, 'specular_color')
  numericParam(specular, 'specular_IOR', 0, 3, 0.0001)
  zeroToOneParam(specular, 'specular_anisotropy')
  zeroToOneParam(specular, 'specular_rotation')
  const subsurface = gui.addFolder('Subsurface')
  zeroToOneParam(subsurface, 'subsurface')
  colorParam(subsurface, 'subsurface_color')
  colorParam(subsurface, 'subsurface_radius')
  numericParam(subsurface, 'subsurface_scale', 0, 10, 0.0001)
  const thin = gui.addFolder('Thin Film')
  numericParam(thin, 'thin_film_thickness', 0, 2000, 1)
  numericParam(thin, 'thin_film_IOR', 0, 3, 0.0001)
  const transmission = gui.addFolder('Transmission')
  zeroToOneParam(transmission, 'transmission')
};

const init = () => {
  const fileloader = new THREE.FileLoader()
  const dracoLoader = new DRACOLoader()
  dracoLoader.setDecoderPath('draco/')
  const gltfLoader = new GLTFLoader()
  gltfLoader.setDRACOLoader(dracoLoader)
  const hdrloader = new RGBELoader()
  const textureLoader = new THREE.TextureLoader()
  Promise.all([
    new Promise(resolve => hdrloader.setDataType(THREE.FloatType).load(getEnvironmentPath(), resolve)),
    new Promise(resolve => fileloader.load('public/lights/cold_lamp.mtlx', resolve)),
    new Promise(resolve => gltfLoader.load(getMeshPath(), resolve)),
    new Promise(resolve => MaterialX().then( module => resolve(module))),
  ]).then(async ([loadedRadianceTexture, loadedLightSetup, { scene: obj }, mxIn]) => {
    mx = mxIn
    materialShader.obj = obj
    // Initialize MaterialX and the shader generation context
    const doc = mx.createDocument()
    const gen = new mx.EsslShaderGenerator()
    const genContext = new mx.GenContext(gen)
    const stdlib = mx.loadStandardLibraries(genContext)
    doc.importLibrary(stdlib)
    const mtlxMaterial = await catchingLoadMaterial(fileloader, getMaterialPath())
    if (mtlxMaterial === undefined) {
      alert(`Material ${getMaterialPath()} not found on server`)
      new FallbackMaterial(mx, doc)
    } else if (getMaterialVersion(mtlxMaterial) < 1.38) {
      alert('3D mode supports only materials version 1.38 or above')
      new FallbackMaterial(mx, doc)
    } else await mx.readFromXmlString(doc, mtlxMaterial)
    let elem = mx.findRenderableElement(doc)
    // Handle transparent materials
    materialShader.isTransparent = mx.isTransparentSurface(elem, gen.getTarget())
    genContext.getOptions().hwTransparency = materialShader.isTransparent
    // Load lighting setup into document
    const lightRigDoc = mx.createDocument()
    await mx.readFromXmlString(lightRigDoc, loadedLightSetup)
    doc.importLibrary(lightRigDoc)
    materialShader.lights = findLights(doc)
    materialShader.lightData = registerLights(mx, materialShader.lights, genContext)
    loadedRadianceTexture.mapping = THREE.EquirectangularReflectionMapping
    //scene.background = loadedRadianceTexture // can be used to show background image
    scene.environment = loadedRadianceTexture
    materialShader.radianceTexture = prepareEnvTexture(loadedRadianceTexture, renderer.capabilities)
    materialShader.shader = gen.generate(elem.getNamePath(), elem, genContext)
    materialShader.uniforms = {
      ...getUniformValues(materialShader.shader.getStage('vertex'), textureLoader),
      ...getUniformValues(materialShader.shader.getStage('pixel'), textureLoader),
    }
    recomputeMaterialVisualisation()
    buildPropertyEditor()
    // Fit camera to model
    const bsphere = new THREE.Sphere()
    new THREE.Box3().setFromObject(model).getBoundingSphere(bsphere)
    controls.target = bsphere.center
    camera.position.y = camera.position.z = bsphere.radius * 1.5
    controls.update()
    camera.far = bsphere.radius * 10
    camera.updateProjectionMatrix()
  })
    .then(() => animate())
    .catch( err => console.error(Number.isInteger(err) ? mx.getExceptionMessage(err) : err) )
}

/**
 * Update camera aspect ratio and on window resize.
 */
const onWindowResize = () => {
  camera.aspect = window.innerWidth / window.innerHeight
  camera.updateProjectionMatrix()
  renderer.setSize(window.innerWidth, window.innerHeight)
}

/**
 * Recompute object visualisation using provided uniforms.
 * @param updatableUniforms {Array[THREE.Uniform]}
 */
const recomputeMaterialVisualisation = () => {
  const { uniforms, lights, lightData, radianceTexture, isTransparent, shader, obj } = materialShader
  Object.assign(uniforms, {
    u_numActiveLightSources: { value: lights.length },
    u_lightData: { value: lightData },
    u_envMatrix: { value: new THREE.Matrix4().makeRotationY(Math.PI) },
    u_envRadiance: { value: radianceTexture },
    u_envRadianceMips: { value: Math.trunc(Math.log2(Math.max(radianceTexture.image.width, radianceTexture.image.height))) + 1 },
    u_envRadianceSamples: { value: 16 }
  })
  const threeMaterial = new THREE.RawShaderMaterial({
    uniforms: uniforms,
    vertexShader: shader.getSourceCode("vertex"),
    fragmentShader: shader.getSourceCode("pixel"),
    transparent: isTransparent,
    blendEquation: THREE.AddEquation,
    blendSrc: THREE.OneMinusSrcAlphaFactor,
    blendDst: THREE.SrcAlphaFactor
  })
  obj.traverse( child => {
    if (child.isMesh) {
      child.geometry.computeTangents()
      child.geometry.attributes.i_position = child.geometry.attributes.position
      child.geometry.attributes.i_normal = child.geometry.attributes.normal
      child.geometry.attributes.i_tangent = child.geometry.attributes.tangent
      child.geometry.attributes.i_texcoord_0 = child.geometry.attributes.uv
      child.material = threeMaterial
    }
  })
  model = obj
  scene.add(model)
}


/**
 * Animate mesh on mouse movement.
 */
const animate = () => {
  requestAnimationFrame(animate)
  composer.render()
  model.traverse((child) => {
    if (child.isMesh) {
      const uniforms = child.material.uniforms
      if (uniforms) {
        uniforms.u_worldMatrix.value = child.matrixWorld
        uniforms.u_viewProjectionMatrix.value = new THREE.Matrix4().multiplyMatrices(camera.projectionMatrix, camera.matrixWorldInverse)
        if (uniforms.u_viewPosition) uniforms.u_viewPosition.value = camera.getWorldPosition(new THREE.Vector3())
        if (uniforms.u_worldInverseTransposeMatrix) uniforms.u_worldInverseTransposeMatrix.value = new THREE.Matrix4().setFromMatrix3(new THREE.Matrix3().getNormalMatrix(child.matrixWorld))
      }
    }
  })
}

/**
 * Compute lens distortion ration based on background image.
 * @param {THREE.Texture} texture
 * @returns {number} ratio for virtual camera
 */
const computeLensDistortionRatio = texture => {
  const img = texture.image
  const ratio = (window.innerWidth / window.innerHeight) / (img.width / img.height)
  texture.repeat = new THREE.Vector2(Math.max(ratio, 1), Math.max(1 / ratio, 1))
  texture.offset = new THREE.Vector2(-Math.max(ratio - 1, 0) / 2, -Math.max(1 / ratio - 1, 0) / 2)
  return ratio
}

setQueryParams()
buildScene('webglcanvas')
init()