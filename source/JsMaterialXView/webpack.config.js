const path = require('path');
const fs = require('fs');
const CopyPlugin = require("copy-webpack-plugin");
const HtmlWebpackPlugin = require('html-webpack-plugin')
const loadResources = (resDir, baseURL) => fs.readdirSync(resDir).map( filename => ({ name: filename, value: `public/resources/${baseURL}/${filename}` }) );

const materials = loadResources("./public/resources/Materials/Examples/StandardSurface", "Materials/Examples/StandardSurface");
const meshes = loadResources("./public/resources/Geometry", "Geometry");
const stdLights = "./public/resources/Lights";
const lights = fs.readdirSync(stdLights)
    .filter( filename => filename.endsWith('.mtlx') )
    .map( filename => {
      const hdr = `${stdLights}/${filename.replace('mtlx', 'hdr')}`;
      const irrad = `${stdLights}/irradiance/${filename.replace('mtlx', 'hdr')}`;
      return (fs.existsSync(hdr) && fs.existsSync(irrad)) ? {
        name: filename,
        value: `public/resources/Lights/${filename}`,
      } : undefined
    })
    .filter( light => light !== undefined )

module.exports = {
  entry: './src/index.js',
  output: { filename: 'main.js', path: path.resolve(__dirname, 'dist') },
  mode: "development",
  plugins: [
    new HtmlWebpackPlugin({
      templateParameters: { materials, meshes, lights },
      template: 'index.ejs'
    }),
    new CopyPlugin({
      patterns: [
        { from: './JsMaterialXGenShader.data', to: 'JsMaterialXGenShader.data' },
        { from: './JsMaterialXGenShader.js', to: 'JsMaterialXGenShader.js' },
        { from: './JsMaterialXGenShader.wasm', to: 'JsMaterialXGenShader.wasm' },
        { from: "./public", to: "public" },
        { from: "node_modules/three/examples/js/libs/draco",  to: "draco" },
      ],
    }),
  ]
};