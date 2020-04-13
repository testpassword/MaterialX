""" Create a USD Material Network using Shader with MaterialX nodedefs
"""
import os
from pxr import Gf, Kind, Sdf, Usd, UsdShade


def addShader(material, name, infoId):
    shader = UsdShade.Shader.Define(
        material.GetPrim().GetStage(), 
        material.GetPath().AppendPath(name))
    shader.CreateIdAttr(infoId)

    return shader

def main():
    stage = Usd.Stage.CreateNew("bamboo_wood_material.usda")

    materialX = stage.DefinePrim('/MaterialX', 'Scope')

    material = UsdShade.Material.Define(stage, materialX.GetPath().AppendPath('material1'))

    standardSurface = addShader(material, 'standardSurface1', 'ND_standard_surface_surfaceshader')
    material.CreateSurfaceOutput().ConnectToSource(standardSurface, "out")
    standardSurface.CreateInput("base", Sdf.ValueTypeNames.Float).Set((1.))
    standardSurface.CreateInput("specular_IOR", Sdf.ValueTypeNames.Float).Set((1.66))

    texturesDirPath = 'textures'
    base_color = addShader(material, 'base_color', 'ND_tiledimage_color3')
    imagePath = os.path.join(texturesDirPath, 'bamboo-wood-semigloss-albedo.png')
    base_color.CreateInput("file", Sdf.ValueTypeNames.Asset).Set(imagePath)
    standardSurface.CreateInput("base_color", Sdf.ValueTypeNames.Color3f).ConnectToSource(base_color, 'out')

    specular_roughness = addShader(material, 'specular_roughness', 'ND_tiledimage_float')
    imagePath = os.path.join(texturesDirPath, 'bamboo-wood-semigloss-roughness.png')
    specular_roughness.CreateInput("file", Sdf.ValueTypeNames.Asset).Set(imagePath)
    standardSurface.CreateInput("specular_roughness", Sdf.ValueTypeNames.Float).ConnectToSource(specular_roughness, 'out')

    normal_image = addShader(material, 'normal_image', 'ND_tiledimage_vector3')
    imagePath = os.path.join(texturesDirPath, 'bamboo-wood-semigloss-normal.png')
    normal_image.CreateInput("file", Sdf.ValueTypeNames.Asset).Set(imagePath)

    normalmap = addShader(material, 'normalmap', 'ND_normalmap')
    normalmap.CreateInput("in", Sdf.ValueTypeNames.Vector3f).ConnectToSource(normal_image, 'out')
    normalmap.CreateInput("scale", Sdf.ValueTypeNames.Float).Set(-1.)
    standardSurface.CreateInput("normal", Sdf.ValueTypeNames.Vector3f).ConnectToSource(normalmap, 'out')

    stage.Save()    

if __name__ == '__main__':
    main()
