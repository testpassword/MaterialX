const path = require('path');
const fs = require('fs');
const CopyPlugin = require("copy-webpack-plugin");
const HtmlWebpackPlugin = require('html-webpack-plugin')
const loadResources = (resDir, baseURL) => fs.readdirSync(resDir).map( filename => ({ name: filename, value: `public/resources/${baseURL}/${filename}` }) );

const meshes = loadResources('public/resources/meshes', 'meshes')
const stdLights = "public/resources/lights";
const lights = fs.readdirSync(stdLights)
  .map( folder => fs.readdirSync(`${stdLights}/${folder}`).map( innerFolder => `${stdLights}/${folder}/${innerFolder}`))
  .flat()
  .filter( filename => filename.endsWith('.mtlx'))
  .map( filename => ({ name: filename.split('/').pop().replace('.mtlx', ''), value: filename }) )

module.exports = {
  entry: './src/index.js',
  output: { filename: 'main.js', path: path.resolve(__dirname, 'dist') },
  mode: "development",
  plugins: [
    new HtmlWebpackPlugin({
      templateParameters: { meshes, lights },
      template: 'index.ejs'
    }),
    new CopyPlugin({
      patterns: [
        { from: './bin/JsMaterialXGenShader.data', to: '.' },
        { from: './bin/JsMaterialXGenShader.js', to: '.' },
        { from: './bin/JsMaterialXGenShader.wasm', to: '.' },
        { from: './public', to: 'public' },
        { from: 'node_modules/three/examples/js/libs/draco',  to: 'draco' },
      ],
    }),
  ]
};