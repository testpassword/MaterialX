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

module.exports = {
  entry: './src/index.js',
  output: {
    filename: 'main.js',
    path: path.resolve(__dirname, 'dist'),
  },
  mode: "development",
  plugins: [
    new HtmlWebpackPlugin({
      templateParameters: { materials, meshes },
      template: 'index.ejs'
    }),
    new CopyPlugin({
      patterns: [
        { context: "./public/resources/Images", from: "*.jpg", to: "Images" },

        { from: "./public/resources/Images/greysphere_calibration.png", to: "Images" },
        { from: "./public/resources/Geometry/shaderball.glb",  to: "Geometry" },
        { from: "./public/resources/Lights/san_giuseppe_bridge_split.hdr", to: "Lights" },
        { from: "./public/resources/Lights/san_giuseppe_bridge_split.mtlx", to: "Lights" },
        { from: "./public/resources/Lights/irradiance/san_giuseppe_bridge_split.hdr", to: "Lights/irradiance" },
        { from: stdSurfaceMaterials, to: stdSurfaceMaterialsBaseURL },
        { from: stdMeshes, to: stdMeshesBaseURL }
      ],
    }),
  ]
};