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
import { prepareEnvTexture, findLights, registerLights, getUniformValues, checkMaterialVersion } from './utils_3D.js'

let camera, scene, model, renderer, composer, controls, mx;
const materialFilename = new URLSearchParams(document.location.search).get("material");
const meshFilename = new URLSearchParams(document.location.search).get("mesh");
const lightFilename = new URLSearchParams(document.location.search).get("light");

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

const setQueryParams = () => {
  let meshesSelect = document.getElementById('meshes');
  meshesSelect.value = meshFilename;
  let lightsSelect = document.getElementById('lights');
  lightsSelect.value = lightFilename;
  meshesSelect.addEventListener('change', e => { window.location.href = `${window.location.origin}${window.location.pathname}?material=${materialFilename}&mesh=${e.target.value}&light=${lightFilename}` });
  lightsSelect.addEventListener('change', e => { window.location.href = `${window.location.origin}${window.location.pathname}?material=${materialFilename}&mesh=${meshFilename}&light=${e.target.value}` });
}

const init = () => {
    setQueryParams();
    checkMaterialVersion(materialFilename, () => {
      let canvas = document.getElementById('webglcanvas');
      let context = canvas.getContext('webgl2');
      camera = new THREE.PerspectiveCamera(50, window.innerWidth / window.innerHeight, 1, 100);
      // Set up scene
      scene = new THREE.Scene();
      scene.background = new THREE.Color(0x4c4c52);
      scene.background.convertSRGBToLinear();
      renderer = new THREE.WebGLRenderer({canvas, context});
      renderer.setSize(window.innerWidth, window.innerHeight);
      composer = new EffectComposer( renderer );
      const renderPass = new RenderPass( scene, camera );
      composer.addPass( renderPass );
      const gammaCorrectionPass = new ShaderPass( GammaCorrectionShader );
      composer.addPass( gammaCorrectionPass );
      window.addEventListener('resize', onWindowResize);
      // controls
      controls = new OrbitControls(camera, renderer.domElement);
      // Load model and shaders
      const fileloader = new THREE.FileLoader();
      const dracoLoader = new DRACOLoader();
      dracoLoader.setDecoderPath( 'draco/' );
      const gltfLoader = new GLTFLoader();
      gltfLoader.setDRACOLoader( dracoLoader );
      const hdrloader = new RGBELoader();
      const textureLoader = new THREE.TextureLoader();
      const hdrLightFilename = lightFilename.replace('mtlx', 'hdr');
      const hdrLightFilenameExploded = hdrLightFilename.split('/');
      hdrLightFilenameExploded.splice(-1, 0, 'irradiance');
      const irradLightFilename = hdrLightFilenameExploded.join('/');
      Promise.all([
        new Promise(resolve => hdrloader.setDataType(THREE.FloatType).load(hdrLightFilename, resolve)),
        new Promise(resolve => fileloader.load(`${lightFilename}`, resolve)),
        new Promise(resolve => hdrloader.setDataType(THREE.FloatType).load(irradLightFilename, resolve)),
        new Promise(resolve => gltfLoader.load(`${meshFilename}`, resolve)),
        new Promise( resolve => MaterialX().then( module => resolve(module))),
        //new Promise(resolve => fileloader.load('public/resources/materials/standard_surface_brass_tiled/standard_surface_brass_tiled.mtlx', resolve))
        new Promise(resolve => materialFilename ? fileloader.load(`${materialFilename}`, resolve) : resolve())
      ]).then(async ([loadedRadianceTexture, loadedLightSetup, loadedIrradianceTexture, {scene: obj}, mxIn, mtlxMaterial]) => {
        // Initialize MaterialX and the shader generation context
        mx = mxIn;
        let doc = mx.createDocument();
        let gen = new mx.EsslShaderGenerator();
        let genContext = new mx.GenContext(gen);
        doc.importLibrary(mx.loadStandardLibraries(genContext));
        // Load material
        if (mtlxMaterial) await mx.readFromXmlString(doc, mtlxMaterial);
        else fallbackMaterial(doc);
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
        let shader = gen.generate(elem.getNamePath(), elem, genContext);
        // Get GL ES shaders and uniform values
        let uniforms = {
          ...getUniformValues(shader.getStage('vertex'), textureLoader),
          ...getUniformValues(shader.getStage('pixel'), textureLoader),
        }
        const radianceTexture = prepareEnvTexture(loadedRadianceTexture, renderer.capabilities);
        const irradianceTexture = prepareEnvTexture(loadedIrradianceTexture, renderer.capabilities);
        Object.assign(uniforms, {
          u_numActiveLightSources: {value: lights.length},
          u_lightData: {value: registerLights(mx, lights, genContext)},
          u_envMatrix: {value: new THREE.Matrix4().makeRotationY(Math.PI)},
          u_envRadiance: {value: radianceTexture},
          u_envRadianceMips: {value: Math.trunc(Math.log2(Math.max(radianceTexture.image.width, radianceTexture.image.height))) + 1},
          u_envRadianceSamples: {value: 16},
          u_envIrradiance: {value: irradianceTexture}
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
        obj.traverse((child) => {
          if (child.isMesh) {
            child.geometry.computeTangents();
            // Use default MaterialX naming convention.
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
        camera.position.y = camera.position.z = bsphere.radius * 2.5;
        controls.update();
        camera.far = bsphere.radius * 10;
        camera.updateProjectionMatrix();
      }).then(() => animate()).catch(err => console.error(Number.isInteger(err) ? mx.getExceptionMessage(err) : err))
    });
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

init();