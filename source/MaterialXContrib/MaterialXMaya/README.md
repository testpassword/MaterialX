# MaterialXMaya

## Description

This module contains code for a sample Maya plug-in which implements Maya surface and texture shading nodes representing MaterialX shaders in Maya's OpenGL viewport (VP2.0). To support this functionality, MaterialX elements are compiled into GLSL code as well as XML shade fragment wrappers required by Maya to integrate the fragments into the final viewport shader.

## Build

To build this plug-in, the `MATERIALX_BUILD_CONTRIB` build flag must be enabled and the respective `MAYA_DEBUG_DIR` and/or `MAYA_RELEASE_DIR` build directory options set. The minimum required Maya version is Maya 2019.

## Maya Nodes

The plug-in defines two Maya nodes: `MaterialXSurfaceNode` and `MaterialXTextureNode`, accessible in Hypershade in the _Surface_ and _2D Textures_ categories, respectively. A new `MaterialXSurfaceNode` can be assigned to a shape via the [Marking Menu](https://knowledge.autodesk.com/support/maya/learn-explore/caas/CloudHelp/cloudhelp/2019/ENU/Maya-LightingShading/files/GUID-D41AF807-F7CB-447E-BACC-7F0867C14E8D-htm.html).

Both node types expose the following attributes:
*   `documentFilePath`: the file path to the MaterialX document (an XML file with extension .mtlx);
*   `elementPath`: the path to the MaterialX element within the document to generate the shader for;
*   `environmentRadianceMap`: the file path to the environment map to use for specular shading, defaults to a texture that ships with the plug-in;
*   `environmentIrradianceMap`: the file path to the environment map to use for diffuse shading, defaults to a texture that ships with the plug-in.

Modifying any one of these attributes via GUI or scripting causes the shading node to refresh in the viewport.

## Maya Commands
### `createMaterialXNode`

Creates one or many MaterialX nodes from the supplied MaterialX document. The command accepts the following flags:

```MEL
   -d -documentFilePath  String
```

Required argument: the path to a valid MaterialX document.

```MEL
   -e -elementPath       String
```
If specified, the command tries to create a node for the given element. If the path is invalid or the element is not renderable, the command fails.

If this argument is omitted, the command creates one node per each renderable element found in the document.

```MEL
  -ei -envIrradiance     String
  -er -envRadiance       String
```

These arguments allow to specify environment maps to use for IBL on the created nodes (see the node documentation). Default assets are used if these arguments are omitted.

```MEL
   -t -asTexture         on|off
```
If specified, determines the type of nodes to create with this command (`MaterialXSurfaceNode` or `MaterialXTextureNode`). If omitted, the type of nodes to create is determined by the type of the MaterialX element (`MaterialXSurfaceNode` for shader refs, `MaterialXTextureNode` otherwise).

```MEL
   -x -ogsXml            String
```
Allows to specify the file path to a Maya XML fragment to use instead of the one auto-generated by MaterialX. For debug purposes only.

### `reloadMaterialXNode`

Reloads the document and refreshes the viewport shader for the MaterialX node with the specified name. Useful in situations when the contents of the document file have changed on disk, e.g. due to editing in LookdevX or an external XML editor.

## MaterialX Libraries

The plug-in loads a set of standard MaterialX libraries (installed together with the plug-in)  at load time. When documents are loaded for particular MaterialX nodes, they import these libraries from memory instead of reading them from the file system.

Users can configure additional library search paths and library directory names with the option variables `materialXLibrarySearchPaths` and `materialXLibraryNames` respectively which both hold arrays of strings. Here's an example of how these variables can be manipulated in MEL:

```MEL
optionVar -stringValueAppend materialXLibrarySearchPaths "D:/MaterialX/Teapot_Demo_MatX"
optionVar -stringValueAppend materialXLibraryNames "stdlib"
```

## LookdevX integration

The plug-in integrates with LookdevX or any other editor that can open MaterialX document files. The `MAYA_MATERIALX_EDITOR` option variable needs to be set to the path of the editor executable to enable the integration, for example:

```MEL
optionVar -stringValue MAYA_MATERIALX_EDITOR "D:/LookdevX/bin/LookdevX.exe"
```

To open the MaterialX document in the editor, press the **Edit** button in the Attribute Editor for the MaterialX node. Once the document has been edited and saved, you can press the **Reload** button the Attribute Editor which will execute the `reloadMaterialXNode` command, refreshing the shader in the viewport.

## Limitations

*   No DirectX11/HLSL support.
*   No support for Maya light sources and shadow maps. Image-based lighting defined by the `environmentRadianceMap` and `environmentIrradianceMap` node attributes is the only type of lighting currently supported.

## Usage

Use of this code is subject to the terms of the Autodesk license agreement provided at the time of installation or download, or which otherwise accompanies this software in either electronic or hard copy form.