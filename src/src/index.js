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
const environmentFilename = new URLSearchParams(document.location.search).get("environment");

const setQueryParams = () => {
  let meshesSelect = document.getElementById('meshes');
  meshesSelect.value = meshFilename;
  let environmentsSelect = document.getElementById('environments');
  environmentsSelect.value = environmentFilename;
  meshesSelect.addEventListener('change', e => { window.location.href = `${window.location.origin}${window.location.pathname}?material=${materialFilename}&mesh=${e.target.value}&environment=${environmentFilename}` });
  environmentsSelect.addEventListener('change', e => { window.location.href = `${window.location.origin}${window.location.pathname}?material=${materialFilename}&mesh=${meshFilename}&environment=${e.target.value}` });
}

const buildScene = canvasID => {
  const canvas = document.getElementById(canvasID);
  camera = new THREE.PerspectiveCamera(40, window.innerWidth / window.innerHeight, 1, 100);
  // Set up scene
  scene = new THREE.Scene();
  renderer = new THREE.WebGLRenderer({ canvas, context: canvas.getContext('webgl2'), antialias: true });
  renderer.setSize(window.innerWidth, window.innerHeight);
  composer = new EffectComposer( renderer );
  composer.addPass( new RenderPass( scene, camera ) );
  composer.addPass( new ShaderPass( GammaCorrectionShader ) );
  window.addEventListener('resize', onWindowResize);
  controls = new OrbitControls(camera, renderer.domElement);
};

const init = () => {
    setQueryParams();
    checkMaterialVersion(materialFilename, () => {
      buildScene('webglcanvas');
      // Load model and shaders
      const fileloader = new THREE.FileLoader();
      const dracoLoader = new DRACOLoader();
      dracoLoader.setDecoderPath( 'draco/' );
      const gltfLoader = new GLTFLoader();
      gltfLoader.setDRACOLoader( dracoLoader );
      const hdrloader = new RGBELoader();
      const textureLoader = new THREE.TextureLoader();
      Promise.all([
        new Promise(resolve => hdrloader.setDataType(THREE.FloatType).load(environmentFilename, resolve)),
        new Promise(resolve => fileloader.load('public/resources/lights/midday_sunshine.mtlx', resolve)),
        new Promise(resolve => gltfLoader.load(meshFilename, resolve)),
        new Promise( resolve => MaterialX().then( module => resolve(module))),
        new Promise(resolve => fileloader.load(materialFilename, resolve))
      ]).then(async ([loadedRadianceTexture, loadedLightSetup, { scene: obj }, mxIn, mtlxMaterial]) => {
        // Initialize MaterialX and the shader generation context
        mx = mxIn;
        let doc = mx.createDocument();
        let gen = new mx.EsslShaderGenerator();
        let genContext = new mx.GenContext(gen);
        doc.importLibrary(mx.loadStandardLibraries(genContext));
        await mx.readFromXmlString(doc, mtlxMaterial);
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
        loadedRadianceTexture.mapping = THREE.EquirectangularReflectionMapping;
        scene.background = loadedRadianceTexture;
        scene.environment = loadedRadianceTexture;
        const radianceTexture = prepareEnvTexture(loadedRadianceTexture, renderer.capabilities);
        Object.assign(uniforms, {
          u_numActiveLightSources: {value: lights.length},
          u_lightData: {value: registerLights(mx, lights, genContext)},
          u_envMatrix: {value: new THREE.Matrix4().makeRotationY(Math.PI)},
          u_envRadiance: {value: radianceTexture},
          u_envRadianceMips: {value: Math.trunc(Math.log2(Math.max(radianceTexture.image.width, radianceTexture.image.height))) + 1},
          u_envRadianceSamples: {value: 16},
        });
        obj.traverse( child => {
          if (child.isMesh) {
            child.geometry.computeTangents();
            // Use default MaterialX naming convention.
            child.geometry.attributes.i_position = child.geometry.attributes.position;
            child.geometry.attributes.i_normal = child.geometry.attributes.normal;
            child.geometry.attributes.i_tangent = child.geometry.attributes.tangent;
            child.geometry.attributes.i_texcoord_0 = child.geometry.attributes.uv;
            child.material = new THREE.RawShaderMaterial({
              uniforms: uniforms,
              vertexShader: shader.getSourceCode("vertex"),
              fragmentShader: shader.getSourceCode("pixel"),
              transparent: isTransparent,
              blendEquation: THREE.AddEquation,
              blendSrc: THREE.OneMinusSrcAlphaFactor,
              blendDst: THREE.SrcAlphaFactor
            });
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
      }).then(() => animate()).catch(err => console.error(err))
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
    model.traverse( child => {
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