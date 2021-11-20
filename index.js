//
// TM & (c) 2021 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved.  See LICENSE.txt for license.
//

import * as THREE from 'three';
import { GLTFLoader } from 'three/examples/jsm/loaders/GLTFLoader';
import { DRACOLoader } from 'three/examples/jsm/loaders/DRACOLoader';
import { RGBELoader } from 'three/examples/jsm/loaders/RGBELoader.js';
import { OrbitControls } from 'three/examples/jsm/controls/OrbitControls.js';
import { EffectComposer } from 'three/examples/jsm/postprocessing/EffectComposer.js';
import { RenderPass } from 'three/examples/jsm/postprocessing/RenderPass.js';
import { ShaderPass } from 'three/examples/jsm/postprocessing/ShaderPass.js';
import { GammaCorrectionShader } from 'three/examples/jsm/shaders/GammaCorrectionShader.js';
import { prepareEnvTexture, findLights, registerLights, getUniformValues, getMaterialVersion } from './mtlx-threejs_converter.js';

let camera, scene, renderer, composer, controls, model, mx;

const getQueryParam = param => new URLSearchParams(document.location.search).get(param);
const getMaterialPath = () => getQueryParam('material') ?? 'public/materials/demo/demo.mtlx';
const getEnvironmentPath = () => getQueryParam('environment') ?? 'public/environments/royal_esplanade.hdr';
const getMeshPath = () => getQueryParam('mesh') ?? 'public/meshes/sphere.glb';

const setQueryParams = () => {
  const meshesSelect = document.getElementById('meshes');
  meshesSelect.value = getMeshPath();
  const environmentsSelect = document.getElementById('environments');
  environmentsSelect.value = getEnvironmentPath();
  meshesSelect.addEventListener('change', e => {
    window.location.href = `${window.location.origin}${window.location.pathname}?material=${getMaterialPath()}&mesh=${e.target.value}&environment=${getEnvironmentPath()}`
  });
  environmentsSelect.addEventListener('change', e => {
    window.location.href = `${window.location.origin}${window.location.pathname}?material=${getMaterialPath()}&mesh=${getMeshPath()}&environment=${e.target.value}`
  });
}

// If no material file is selected, we programmatically create a jade material as a fallback
const fallbackMaterial = doc => {
  const ssName = 'SR_default';
  const ssNode = doc.addChildOfCategory('standard_surface', ssName);
  ssNode.setType('surfaceshader');
  ssNode.setInputValueFloat('base', 1.0);
  ssNode.setInputValueColor3('base_color', new mx.Color3(0.8, 0.8, 0.8));
  ssNode.setInputValueFloat('diffuse_roughness', 0);
  ssNode.setInputValueFloat('specular', 1);
  ssNode.setInputValueColor3('specular_color', new mx.Color3(1, 1, 1));
  ssNode.setInputValueFloat('specular_roughness', 0.2);
  ssNode.setInputValueFloat('specular_IOR', 1.5);
  ssNode.setInputValueFloat('specular_anisotropy', 0);
  ssNode.setInputValueFloat('specular_rotation', 0);
  ssNode.setInputValueFloat('metalness', 0);
  ssNode.setInputValueFloat('transmission', 0);
  ssNode.setInputValueColor3('transmission_color', new mx.Color3(1, 1, 1));
  ssNode.setInputValueFloat('transmission_depth', 0);
  ssNode.setInputValueColor3('transmission_scatter', new mx.Color3(0, 0, 0));
  ssNode.setInputValueFloat('transmission_scatter_anisotropy', 0);
  ssNode.setInputValueFloat('transmission_dispersion', 0);
  ssNode.setInputValueFloat('transmission_extra_roughness', 0);
  ssNode.setInputValueFloat('subsurface', 0)
  ssNode.setInputValueColor3('subsurface_color', new mx.Color3(1, 1, 1));
  ssNode.setInputValueColor3('subsurface_radius', new mx.Color3(1, 1, 1));
  ssNode.setInputValueFloat('subsurface_scale', 1);
  ssNode.setInputValueFloat('subsurface_anisotropy', 0);
  ssNode.setInputValueFloat('sheen', 0);
  ssNode.setInputValueColor3('sheen_color', new mx.Color3(1, 1, 1));
  ssNode.setInputValueFloat('sheen_roughness', 0.3);
  ssNode.setInputValueBoolean('thin_walled', false);
  ssNode.setInputValueFloat('coat', 0);
  ssNode.setInputValueColor3('coat_color', new mx.Color3(1, 1, 1));
  ssNode.setInputValueFloat('coat_roughness', 0.1);
  ssNode.setInputValueFloat('coat_anisotropy', 0.0);
  ssNode.setInputValueFloat('coat_rotation', 0.0);
  ssNode.setInputValueFloat('coat_IOR', 1.5);
  ssNode.setInputValueFloat('coat_affect_color', 0);
  ssNode.setInputValueFloat('coat_affect_roughness', 0);
  ssNode.setInputValueFloat('thin_film_thickness', 0);
  ssNode.setInputValueFloat('thin_film_IOR', 1.5);
  ssNode.setInputValueFloat('emission', 0);
  ssNode.setInputValueColor3('emission_color', new mx.Color3(1, 1, 1));
  ssNode.setInputValueColor3('opacity', new mx.Color3(1, 1, 1));
  const smNode = doc.addChildOfCategory('surfacematerial', 'Default');
  smNode.setType('material');
  const shaderElement = smNode.addInput('surfaceshader');
  shaderElement.setType('surfaceshader');
  shaderElement.setNodeName(ssName);
}

const buildScene = canvasId => {
  const canvas = document.getElementById(canvasId);
  camera = new THREE.PerspectiveCamera(45, window.innerWidth / window.innerHeight, 1, 100);
  scene = new THREE.Scene();
  renderer = new THREE.WebGLRenderer({ canvas, context: canvas.getContext('webgl2'), antialias: true });
  renderer.setSize(window.innerWidth, window.innerHeight);
  composer = new EffectComposer(renderer);
  composer.addPass(new RenderPass(scene, camera));
  composer.addPass(new ShaderPass( GammaCorrectionShader ));
  window.addEventListener('resize', onWindowResize);
  controls = new OrbitControls(camera, renderer.domElement);
};

const init = () => {
  buildScene('webglcanvas');
  const fileloader = new THREE.FileLoader();
  const dracoLoader = new DRACOLoader();
  dracoLoader.setDecoderPath('draco/');
  const gltfLoader = new GLTFLoader();
  gltfLoader.setDRACOLoader(dracoLoader);
  const hdrloader = new RGBELoader();
  const textureLoader = new THREE.TextureLoader();
  Promise.all([
    new Promise(resolve => hdrloader.setDataType(THREE.FloatType).load(getEnvironmentPath(), resolve)),
    new Promise(resolve => fileloader.load('public/lights/midday_sunshine.mtlx', resolve)),
    new Promise(resolve => gltfLoader.load(getMeshPath(), resolve)),
    new Promise(resolve => MaterialX().then( module => resolve(module))),
    new Promise(resolve => fileloader.load(getMaterialPath(), resolve)),
  ]).then(async ([loadedRadianceTexture, loadedLightSetup, { scene: obj }, mx, mtlxMaterial]) => {
    // Initialize MaterialX and the shader generation context
    let doc = mx.createDocument();
    let gen = new mx.EsslShaderGenerator();
    let genContext = new mx.GenContext(gen);
    let stdlib = mx.loadStandardLibraries(genContext);
    doc.importLibrary(stdlib);
    // Load material
    if (mtlxMaterial && getMaterialVersion(mtlxMaterial) >= 1.38) await mx.readFromXmlString(doc, mtlxMaterial);
    else {
      fallbackMaterial(doc);
      alert('3D mode supports only materials version 1.38 or above');
      // TODO: большая надпись, что поддерживается 1.38 и выше
    }
    let elem = mx.findRenderableElement(doc);
    // Handle transparent materials
    const isTransparent = mx.isTransparentSurface(elem, gen.getTarget());
    genContext.getOptions().hwTransparency = isTransparent;
    // Load lighting setup into document
    const lightRigDoc = mx.createDocument();
    await mx.readFromXmlString(lightRigDoc, loadedLightSetup);
    doc.importLibrary(lightRigDoc);
    // Register lights with generation context
    const lights = findLights(doc);
    const lightData = registerLights(mx, lights, genContext);
    let shader = gen.generate(elem.getNamePath(), elem, genContext);
    let uniforms = {
      ...getUniformValues(shader.getStage('vertex'), textureLoader),
      ...getUniformValues(shader.getStage('pixel'), textureLoader),
    }
    loadedRadianceTexture.mapping = THREE.EquirectangularReflectionMapping;
    scene.background = loadedRadianceTexture;
    scene.environment = loadedRadianceTexture;
    const radianceTexture = prepareEnvTexture(loadedRadianceTexture, renderer.capabilities);
    Object.assign(uniforms, {
      u_numActiveLightSources: { value: lights.length },
      u_lightData: { value: lightData },
      u_envMatrix: { value: new THREE.Matrix4().makeRotationY(Math.PI) },
      u_envRadiance: { value: radianceTexture },
      u_envRadianceMips: { value: Math.trunc(Math.log2(Math.max(radianceTexture.image.width, radianceTexture.image.height))) + 1 },
      u_envRadianceSamples: { value: 16 }
    });
    // Create Three JS Material
    const threeMaterial = new THREE.RawShaderMaterial({
      uniforms: uniforms,
      vertexShader: shader.getSourceCode("vertex"),
      fragmentShader: shader.getSourceCode("pixel"),
      transparent: isTransparent,
      blendEquation: THREE.AddEquation,
      blendSrc: THREE.OneMinusSrcAlphaFactor,
      blendDst: THREE.SrcAlphaFactor
    });
    obj.traverse( child => {
      if (child.isMesh) {
        child.geometry.computeTangents();
        child.geometry.attributes.i_position = child.geometry.attributes.position;
        child.geometry.attributes.i_normal = child.geometry.attributes.normal;
        child.geometry.attributes.i_tangent = child.geometry.attributes.tangent;
        child.geometry.attributes.i_texcoord_0 = child.geometry.attributes.uv;
        child.material = threeMaterial;
      }
    });
    model = obj;
    scene.add(model);
    // Fit camera to model
    const bsphere = new THREE.Sphere();
    new THREE.Box3().setFromObject(model).getBoundingSphere(bsphere);
    controls.target = bsphere.center;
    camera.position.y = camera.position.z = bsphere.radius * 1.5;
    controls.update();
    camera.far = bsphere.radius * 10;
    camera.updateProjectionMatrix();
  })
    .then(() => animate())
    .catch( err => console.error(Number.isInteger(err) ? mx.getExceptionMessage(err) : err) );
}

const onWindowResize = () => {
  camera.aspect = window.innerWidth / window.innerHeight;
  camera.updateProjectionMatrix();
  renderer.setSize(window.innerWidth, window.innerHeight);
}

const animate = () => {
  requestAnimationFrame(animate);
  composer.render();
  model.traverse((child) => {
    if (child.isMesh) {
      const uniforms = child.material.uniforms;
      if (uniforms) {
        uniforms.u_worldMatrix.value = child.matrixWorld;
        uniforms.u_viewProjectionMatrix.value = new THREE.Matrix4().multiplyMatrices(camera.projectionMatrix, camera.matrixWorldInverse);
        if (uniforms.u_viewPosition) uniforms.u_viewPosition.value = camera.getWorldPosition(new THREE.Vector3());
        if (uniforms.u_worldInverseTransposeMatrix) uniforms.u_worldInverseTransposeMatrix.value = new THREE.Matrix4().setFromMatrix3(new THREE.Matrix3().getNormalMatrix(child.matrixWorld));
      }
    }
  });
}

setQueryParams();
init();