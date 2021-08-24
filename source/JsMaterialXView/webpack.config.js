const path = require('path');
const fs = require('fs');
const CopyPlugin = require("copy-webpack-plugin");
const HtmlWebpackPlugin = require('html-webpack-plugin')
const loadResources = (resDir, baseURL) => fs.readdirSync(resDir).map( filename => ({ name: filename, value: `${baseURL}/${filename}` }) );

const stdSurfaceMaterials = "./public/resources/Materials/Examples/StandardSurface";
const stdSurfaceMaterialsBaseURL = "Materials/Examples/StandardSurface";
const materials = loadResources(stdSurfaceMaterials, stdSurfaceMaterialsBaseURL);

const stdMeshes = "./public/resources/Geometry";
const stdMeshesBaseURL = "Geometry";
const meshes = loadResources(stdMeshes, stdMeshesBaseURL);

const stdLights = "./public/resources/Lights";
const stdLightsBaseURL = "Lights";
const lights = fs.readdirSync(stdLights)
    .filter( filename => filename.endsWith('.mtlx') )
    .map( filename => {
      const hdr = `${stdLights}/${filename.replace('mtlx', 'hdr')}`;
      const irrad = `${stdLights}/irradiance/${filename.replace('mtlx', 'hdr')}`;
      return (fs.existsSync(hdr) && fs.existsSync(irrad)) ? {
        name: filename,
        value: `${stdLightsBaseURL}/${filename}`,
        hdr,
        irrad
      } : undefined
    })
    .filter( light => light !== undefined )

module.exports = {
  entry: './src/index.js',
  output: {
    filename: 'main.js',
    path: path.resolve(__dirname, 'dist'),
  },
  mode: "development",
  plugins: [
    new HtmlWebpackPlugin({
      templateParameters: { materials, meshes, lights },
      template: 'index.ejs'
    }),
    new CopyPlugin({
      patterns: [
        { context: "./public/resources/Images", from: "*.jpg", to: "Images" },
        { from: "./public/resources/Images/greysphere_calibration.png", to: "Images" },
        { from: stdLights, to: stdLightsBaseURL },
        { from: stdSurfaceMaterials, to: stdSurfaceMaterialsBaseURL },
        { from: stdMeshes, to: stdMeshesBaseURL }
      ],
    }),
  ]
};