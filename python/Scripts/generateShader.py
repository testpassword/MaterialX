#!/usr/bin/env python
'''
Utility to generate the shader for materials found in a MaterialX document. One file will be generated
for each material / shader found. The currently supported target languages are GLSL, OSL and MDL.
'''

from modulefinder import ModuleFinder
import sys, os, argparse
import MaterialX as mx
import MaterialX.PyMaterialXGenShader as mx_gen_shader
import MaterialX.PyMaterialXGenGlsl as mx_gen_glsl
import MaterialX.PyMaterialXGenOsl as mx_gen_osl
import MaterialX.PyMaterialXGenMdl as mx_gen_mdl

def main():
    parser = argparse.ArgumentParser(description='Generate shader code for each material / shader in a document.')
    parser.add_argument('--path', dest='paths', action='append', nargs='+', help='An additional absolute search path location (e.g. "/projects/MaterialX")')
    parser.add_argument('--library', dest='libraries', action='append', nargs='+', help='An additional relative path to a custom data library folder (e.g. "libraries/custom")')
    parser.add_argument('--target', dest='target', default='glsl', help='Target shader generator to use (e.g. "genglsl"')
    parser.add_argument('--outputFilepath', dest='outputFilepath', help='File path to output shaders to.')
    parser.add_argument(dest='inputFilename', help='Filename of the input document.')
    opts = parser.parse_args()

    doc = mx.createDocument()
    try:
        mx.readFromXmlFile(doc, opts.inputFilename)
    except mx.ExceptionFileMissing as err:
        print(err)
        sys.exit(0)

    stdlib = mx.createDocument()
    filePath = os.path.dirname(os.path.abspath(__file__))
    searchPath = mx.FileSearchPath(os.path.join(filePath, '..', '..', 'libraries'))
    searchPath.append(os.path.dirname(opts.inputFilename))
    libraryFolders = [ 'libraries' ]
    if opts.paths:
        for pathList in opts.paths:
            for path in pathList:
                searchPath.append(path)
    if opts.libraries:
        for libraryList in opts.libraries:
            for library in libraryList:
                libraryFolders.append(library)
    mx.loadLibraries(libraryFolders, searchPath, stdlib)
    doc.importLibrary(stdlib)

    valid, msg = doc.validate()
    if not valid:
        print('Validation warnings for input document:')
        print(msg)

    gentarget = 'glsl'
    if opts.target:
        gentarget = opts.target
    if gentarget == 'osl':
        shadergen = mx_gen_osl.OslShaderGenerator.create()
    elif gentarget == 'mdl':
        shadergen = mx_gen_mdl.MdlShaderGenerator.create()
    else:
        shadergen = mx_gen_glsl.GlslShaderGenerator.create()
    context = mx_gen_shader.GenContext(shadergen)
    context.registerSourceCodeSearchPath(searchPath)
    genoptions = context.getOptions() 
    genoptions.shaderInterfaceType = int(mx_gen_shader.ShaderInterfaceType.SHADER_INTERFACE_COMPLETE)

    try:
        cms = mx_gen_shader.OCIOColorManagementSystem.create(shadergen.getTarget())  
    except:
        cms = mx_gen_shader.DefaultColorManagementSystem.create(shadergen.getTarget())  
    print('- Set up CMS: ' + cms.getName())
    cms.loadLibrary(doc)
    shadergen.setColorManagementSystem(cms)  

    print('- Set up units...')
    unitsystem = mx_gen_shader.UnitSystem.create(shadergen.getTarget())
    registry = mx.UnitConverterRegistry.create()
    distanceTypeDef = doc.getUnitTypeDef('distance')
    registry.addUnitConverter(distanceTypeDef, mx.LinearUnitConverter.create(distanceTypeDef))
    angleTypeDef = doc.getUnitTypeDef('angle')
    registry.addUnitConverter(angleTypeDef, mx.LinearUnitConverter.create(angleTypeDef))
    unitsystem.loadLibrary(stdlib)
    unitsystem.setUnitConverterRegistry(registry)
    shadergen.setUnitSystem(unitsystem)
    genoptions.targetDistanceUnit = 'meter'

    # This seems to always return 0 elements ?
    #renderables = []
    #mx_gen_shader.findRenderableElements(doc, renderables, False)
    materials = doc.getMaterialNodes()
    shaderNodes = []
    for material in materials:       
        shaderNodes = mx.getShaderNodes(material)

    #if len(shaderNodes) == 0:
    #    shaderNodes = doc.getNodesOfType

    pathPrefix = ''
    if opts.outputFilepath and os.path.exists(opts.outputFilepath):
        pathPrefix = opts.outputFilepath

    for shaderNode in shaderNodes:
        shaderNodeName = shaderNode.getName()
        shaderNodeName = mx.createValidName(shaderNodeName)
        shader = shadergen.generate(shaderNodeName, shaderNode, context)        
        if shader:
            pixelSource = shader.getSourceCode(mx_gen_shader.PIXEL_STAGE)
            filename = pathPrefix + shader.getName() + "." + gentarget
            print('-- Wrote pixel shader to: ' + filename)
            file = open(filename, 'w+')
            file.write(pixelSource)
            file.close()                   

            if gentarget == 'glsl':            
                vertexSource = shader.getSourceCode(mx_gen_shader.VERTEX_STAGE)
                filename = pathPrefix + shader.getName() + '_vs.' + gentarget
                print('-- Wrote vertex shader to: ' + filename)
                file = open(filename, 'w+')
                file.write(vertexSource)
                file.close()                    
        else:
            print('Failed to generate code for: ' + shaderNode.getName())


if __name__ == '__main__':
    main()
