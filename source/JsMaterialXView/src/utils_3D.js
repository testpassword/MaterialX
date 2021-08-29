//
// TM & (c) 2021 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved.  See LICENSE.txt for license.
//

import * as THREE from 'three';

const IMAGE_PROPERTY_SEPARATOR = "_";
const UADDRESS_MODE_SUFFIX = IMAGE_PROPERTY_SEPARATOR + "uaddressmode";
const VADDRESS_MODE_SUFFIX = IMAGE_PROPERTY_SEPARATOR + "vaddressmode";
const FILTER_TYPE_SUFFIX = IMAGE_PROPERTY_SEPARATOR + "filtertype";
const FILE_PREFIX = '../public/resources/Images/';
const TARGET_FILE_PREFIX = 'Images/';

/**
 * Initialized the environment texture as MaterialX expects it
 * @param {THREE.Texture} texture
 * @param {Object} capabilities
 * @returns {THREE.Texture}
 */
const prepareEnvTexture = (texture, capabilities) => {
  const rgbaTexture = RGBToRGBA_Float(texture);
  // RGBELoader sets flipY to true by default
  rgbaTexture.flipY = false;
  rgbaTexture.wrapS = THREE.RepeatWrapping;
  rgbaTexture.anisotropy = capabilities.getMaxAnisotropy();
  rgbaTexture.minFilter = THREE.LinearMipmapLinearFilter;
  rgbaTexture.magFilter = THREE.LinearFilter;
  rgbaTexture.generateMipmaps = true;
  rgbaTexture.needsUpdate = true;
  return rgbaTexture;
}

/**
 * Create a new (half)float texture containing an alpha channel with a value of 1 from a RGB (half)float texture.
 * @param {THREE.Texture} texture
 */
const RGBToRGBA_Float = (texture) => {
  const rgbData = texture.image.data;
  const length = (rgbData.length / 3) * 4;
  let rgbaData;
  switch (texture.type) {
    case THREE.FloatType:
      rgbaData = new Float32Array(length);
      break;
    case THREE.HalfFloatType:
      rgbaData = new Uint16Array(length);
      break;
    default:
      break;
  }
  if (rgbaData) {
    for (let i = 0; i < length / 4; i++) {
      rgbaData[(i * 4) + 0] = rgbData[(i * 3) + 0];
      rgbaData[(i * 4) + 1] = rgbData[(i * 3) + 1];
      rgbaData[(i * 4) + 2] = rgbData[(i * 3) + 2];
      rgbaData[(i * 4) + 3] = 1.0;
    }
    return new THREE.DataTexture(rgbaData, texture.image.width, texture.image.height, THREE.RGBAFormat, texture.type);
  }
  return texture;
}

const fromVector = (value, dimension) => {
  let outValue;
  if (value) outValue = value.data();
  else {
    outValue = [];
    for (let i = 0; i < dimension; ++i) outValue.push(0.0);
  }
  return outValue;
}

const fromMatrix = (matrix, dimension) => {
  let vec = [];
  if (matrix)
    for (let i = 0; i < matrix.numRows(); ++i)
      for (let k = 0; k < matrix.numColumns(); ++k)
        vec.push(matrix.getItem(i, k));
  else for (let i = 0; i < dimension; ++i) vec.push(0.0);
  return vec;
}

const toThreeUniform = (type, value, name, uniforms, textureLoader) => {
  let outValue;
  switch(type) {
    case 'float':
    case 'integer':
    case 'boolean':
      outValue = value;
      break;
    case 'vector2':
      outValue = fromVector(value, 2);
      break;
    case 'vector3':
    case 'color3':
      outValue = fromVector(value, 3);
      break;
    case 'vector4':
    case 'color4':
      outValue = fromVector(value, 4);
      break;
    case 'matrix33':
      outValue = fromMatrix(value, 9);
      break;
    case 'matrix44':
      outValue = fromMatrix(value, 16);
      break;
    case 'filename':
      if (value) {
        const mappedValue = value.replace(FILE_PREFIX, TARGET_FILE_PREFIX)
        const texture = textureLoader.load(mappedValue);
        // Set address & filtering mode
        setTextureParameters(texture, name, uniforms);
        outValue = texture;
      }
      break;
    case 'samplerCube':
    case 'string':
      break;
    default:
      // struct
      outValue = toThreeUniform(value);
  }
  return outValue;
}

const getWrapping = (mode) => {
  let wrap;
  switch(mode) {
    case 1:
      wrap = THREE.ClampToEdgeWrapping;
      break;
    case 2:
      wrap = THREE.RepeatWrapping;
      break;
    case 3:
      wrap = THREE.MirroredRepeatWrapping;
      break;
    default:
      wrap = THREE.RepeatWrapping;
      break;
  }
  return wrap;
}

const getMinFilter = (type, generateMipmaps) => {
  let filterType = generateMipmaps ? THREE.LinearMipMapLinearFilter : THREE.LinearFilter;
  if (type === 0) filterType = generateMipmaps ? THREE.NearestMipMapNearestFilter : THREE.NearestFilter;
  return filterType;
}

const setTextureParameters = (texture, name, uniforms, generateMipmaps = true) => {
  const base = name.substring(0, name.lastIndexOf(IMAGE_PROPERTY_SEPARATOR)) || name;
  texture.generateMipmaps = generateMipmaps;
  texture.wrapS = getWrapping(uniforms.find(base + UADDRESS_MODE_SUFFIX)?.getValue().getData());
  texture.wrapT = getWrapping(uniforms.find(base + VADDRESS_MODE_SUFFIX)?.getValue().getData());
  texture.magFilter = THREE.LinearFilter;
  texture.minFilter = getMinFilter(uniforms.get(base + FILTER_TYPE_SUFFIX) ? uniforms.get(base + FILTER_TYPE_SUFFIX).value : -1, generateMipmaps);
}

/**
 * Returns all lights nodes in the document
 * @param {mx.Document} doc
 * @returns {Array.<mx.Node>}
 */
const findLights = (doc) => {
  let lights = [];
  for (let node of doc.getNodes())
    if (node.getType() === "lightshader") lights.push(node);
  return lights;
}

/**
 * Register lights in shader generation context
 * @param {Object} mx MaterialX Module
 * @param {Array.<mx.Node>} lights Light nodes
 * * @param {mx.GenContext} genContext Shader generation context
 * @returns {Array.<mx.Node>}
 */
const registerLights = (mx, lights, genContext) => {
  mx.HwShaderGenerator.unbindLightShaders(genContext);
  const lightTypesBound = {};
  const lightData = [];
  let lightId = 1;
  for (let light of lights) {
    let nodeDef = light.getNodeDef();
    let nodeName = nodeDef.getName();
    if (!lightTypesBound[nodeName]) {
      lightTypesBound[nodeName] = lightId;
      mx.HwShaderGenerator.bindLightShader(nodeDef, lightId++, genContext);
    }
    const lightDirection = light.getValueElement("direction").getValue().getData().data();
    const lightColor = light.getValueElement("color").getValue().getData().data();
    const lightIntensity = light.getValueElement("intensity").getValue().getData();
    lightData.push({
      type: lightTypesBound[nodeName],
      direction: new THREE.Vector3(...lightDirection),
      color: new THREE.Vector3(...lightColor),
      intensity: lightIntensity
    });
  }
  // Make sure max light count is large enough
  genContext.getOptions().hwMaxActiveLightSources = Math.max(genContext.getOptions().hwMaxActiveLightSources, lights.length);
  return lightData;
}

const getUniformValues = (shaderStage, textureLoader) => {
  let threeUniforms = {};
  Object.values(shaderStage.getUniformBlocks()).forEach(uniforms => {
    if (!uniforms.empty())
      for (let i = 0; i < uniforms.size(); ++i) {
        const variable = uniforms.get(i);
        const value = variable.getValue()?.getData();
        const name = variable.getVariable();
        threeUniforms[name] = new THREE.Uniform(toThreeUniform(variable.getType().getName(), value, name, uniforms, textureLoader));
      }
  });
  return threeUniforms;
}

const checkMaterialVersion = (materialFilename, allowedVersions = ['1.37, 1.38'], onSuccess, onError) => {
  fetch(`${window.location.origin}/${materialFilename}`, { method: 'GET' })
    .then( res => res.text() )
    .then( rawFile => {
      ((allowedVersions.includes(
        new DOMParser().parseFromString(rawFile, "application/xml")
          .getElementsByTagName('materialx')[0]
          .getAttribute('version'))
      ) ? onSuccess: onError)()
    })
    .catch( err => console.error(`Failed to load resource ${materialFilename} cause ${err}`) )
}

export { prepareEnvTexture, findLights, registerLights, getUniformValues, checkMaterialVersion };