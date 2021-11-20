const path = require('path');
const fs = require('fs');
const CopyPlugin = require("copy-webpack-plugin");
const HtmlWebpackPlugin = require('html-webpack-plugin');

const loadResources = (resDir, baseURL) => fs.readdirSync(resDir).map( filename => ({ name: filename, value: `public/${baseURL}/${filename}` }) );
const meshes = loadResources('public/meshes', 'meshes');
const environments = loadResources('public/environments', 'environments');

module.exports = {
  entry: './index.js',
  output: {
    filename: 'main.js',
    path: path.resolve(__dirname, 'dist')
  },
  mode: "development",
  plugins: [
    new HtmlWebpackPlugin({
      templateParameters: { meshes, environments },
      template: 'index.ejs'
    }),
    new CopyPlugin({
      patterns: [
        { from: "./public/", to: 'public' },
        { from: "node_modules/three/examples/js/libs/draco",  to: "draco" },
        { from: "./bin/", to: "." },
        { from: "./dependencies", to: "." }
      ],
    }),
  ]
};