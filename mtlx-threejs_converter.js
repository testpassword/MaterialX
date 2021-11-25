import * as THREE from 'three'

const IMAGE_PROPERTY_SEPARATOR = "_"
const UADDRESS_MODE_SUFFIX = IMAGE_PROPERTY_SEPARATOR + "uaddressmode"
const VADDRESS_MODE_SUFFIX = IMAGE_PROPERTY_SEPARATOR + "vaddressmode"

/**
 * Initialized the environment texture as MaterialX expects it
 * @param {THREE.Texture} texture
 * @param {Object} capabilities
 * @returns {THREE.Texture}
 */
const prepareEnvTexture = (texture, capabilities) => {
  const rgbaTexture = RGBToRGBA_Float(texture)
  rgbaTexture.flipY = false
  rgbaTexture.wrapS = THREE.RepeatWrapping
  rgbaTexture.anisotropy = capabilities.getMaxAnisotropy()
  rgbaTexture.minFilter = THREE.LinearMipmapLinearFilter
  rgbaTexture.magFilter = THREE.LinearFilter
  rgbaTexture.generateMipmaps = true
  rgbaTexture.needsUpdate = true
  return rgbaTexture
}

/**
 * Create a new (half)float texture containing an alpha channel with a value of 1 from a RGB (half)float texture.
 * @param {THREE.Texture} texture
 */
const RGBToRGBA_Float = texture => {
  const rgbData = texture.image.data
  const length = (rgbData.length / 3) * 4
  let rgbaData
  switch (texture.type) {
    case THREE.FloatType:
      rgbaData = new Float32Array(length)
      break
    case THREE.HalfFloatType:
      rgbaData = new Uint16Array(length)
      break
    default:
      break
  }
  if (rgbaData) {
    for (let i = 0; i < length / 4; i++) {
      rgbaData[(i * 4) + 0] = rgbData[(i * 3) + 0]
      rgbaData[(i * 4) + 1] = rgbData[(i * 3) + 1]
      rgbaData[(i * 4) + 2] = rgbData[(i * 3) + 2]
      rgbaData[(i * 4) + 3] = 1.0
    }
    return new THREE.DataTexture(rgbaData, texture.image.width, texture.image.height, THREE.RGBAFormat, texture.type)
  }
  return texture
}

const fromVector = (value, dimension) => (value) ? value.data() : Array(dimension).fill(0.0)

const fromMatrix = (matrix, dimension) => {
  let vec = []
  if (matrix)
    for (let i = 0; i < matrix.numRows(); ++i)
      for (let k = 0; k < matrix.numColumns(); ++k)
        vec.push(matrix.getItem(i, k))
  else
    for (let i = 0; i < dimension; ++i)
      vec.push(0.0)
  return vec
}

const toThreeUniform = (type, value, name, uniforms, textureLoader) => {
  switch (type) {
    case 'float':
    case 'integer':
    case 'boolean':
      return value;
    case 'vector2':
      return fromVector(value, 2)
    case 'vector3':
    case 'color3':
      return fromVector(value, 3)
    case 'vector4':
    case 'color4':
      return fromVector(value, 4)
    case 'matrix33':
      return fromMatrix(value, 9)
    case 'matrix44':
      return fromMatrix(value, 16)
    case 'filename':
      if (value) {
        const texture = textureLoader.load(value)
        setTextureParameters(texture, name, uniforms)
        return texture
      } else return undefined
    case 'samplerCube':
    case 'string':
      return undefined
    default:
      return toThreeUniform(value)
  }
}

const getWrapping = mode => {
  switch (mode) {
    case 1:
      return THREE.ClampToEdgeWrapping
    case 2:
      return THREE.RepeatWrapping
    case 3:
      return THREE.MirroredRepeatWrapping
    default:
      return THREE.RepeatWrapping
  }
}

const getMinFilter = (type, generateMipmaps) =>
  (type === 0) ?
    (generateMipmaps ?
      THREE.NearestMipMapNearestFilter :
      THREE.NearestFilter
    ) :
    (generateMipmaps ?
      THREE.LinearMipMapLinearFilter :
      THREE.LinearFilter
    )

const setTextureParameters = (texture, name, uniforms, generateMipmaps = true) => {
  const idx = name.lastIndexOf(IMAGE_PROPERTY_SEPARATOR)
  const base = name.substring(0, idx) || name
  texture.generateMipmaps = generateMipmaps
  const uaddressmode = uniforms.find(base + UADDRESS_MODE_SUFFIX)?.getValue().getData()
  const vaddressmode = uniforms.find(base + VADDRESS_MODE_SUFFIX)?.getValue().getData()
  texture.wrapS = getWrapping(uaddressmode)
  texture.wrapT = getWrapping(vaddressmode)
  texture.magFilter = THREE.LinearFilter
  texture.minFilter = getMinFilter(base, generateMipmaps)
}

/**
 * Returns all lights nodes in the document
 * @param {mx.Document} doc
 * @returns {Array.<mx.Node>}
 */
const findLights = doc => doc.getNodes().filter(it => it.getType() === "lightshader")

/**
 * Register lights in shader generation context
 * @param {Object} mx MaterialX Module
 * @param {Array.<mx.Node>} lights Light nodes
 * @param {mx.GenContext} genContext Shader generation context
 * @returns {Array.<mx.Node>}
 */
const registerLights = (mx, lights, genContext) => {
  mx.HwShaderGenerator.unbindLightShaders(genContext)
  const lightTypesBound = {}
  const lightData = []
  let lightId = 1
  for (let light of lights) {
    let nodeDef = light.getNodeDef()
    let nodeName = nodeDef.getName()
    if (!lightTypesBound[nodeName]) {
      lightTypesBound[nodeName] = lightId
      mx.HwShaderGenerator.bindLightShader(nodeDef, lightId++, genContext)
    }
    const lightDirection = light.getValueElement("direction").getValue().getData().data()
    const lightColor = light.getValueElement("color").getValue().getData().data()
    const lightIntensity = light.getValueElement("intensity").getValue().getData()
    lightData.push({
      type: lightTypesBound[nodeName],
      direction: new THREE.Vector3(...lightDirection),
      color: new THREE.Vector3(...lightColor),
      intensity: lightIntensity
    })
  }
  // Make sure max light count is large enough
  genContext.getOptions().hwMaxActiveLightSources = Math.max(genContext.getOptions().hwMaxActiveLightSources, lights.length)
  return lightData
}

const getUniformValues = (shaderStage, textureLoader) => {
  let threeUniforms = {}
  Object.values(shaderStage.getUniformBlocks())
    .filter(uniform => !uniform.empty())
    .forEach(uniforms => {
      for (let i = 0; i < uniforms.size(); ++i) {
        const variable = uniforms.get(i)
        const value = variable.getValue()?.getData()
        const name = variable.getVariable()
        threeUniforms[name] = new THREE.Uniform(toThreeUniform(variable.getType().getName(), value, name, uniforms, textureLoader))
      }
    })
  return threeUniforms
}

/**
 * Returns mtlx-material version
 * @param material {string} string with xml graph
 * @returns {number} material version
 */
const getMaterialVersion = material =>
  parseFloat(
    new DOMParser()
      .parseFromString(material, "application/xml")
      .getElementsByTagName('materialx')[0]
      .getAttribute('version')
  )

export { prepareEnvTexture, findLights, registerLights, getUniformValues, getMaterialVersion }