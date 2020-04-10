import os
from pxr import Gf, Kind, Sdf, Usd, UsdShade

stage = Usd.Stage.CreateNew("node_graphs.usda")

materialX = stage.DefinePrim('/MaterialX', 'Scope')


material = UsdShade.Material.Define(stage, materialX.GetPath().AppendPath('material1'))

image1 = UsdShade.Shader.Define(stage, material.GetPath().AppendPath('image1'))
image1.CreateIdAttr('ND_image_color3')

imagePath = os.path.join(os.path.dirname(__file__), '..', '..', '..', '..')
imagePath = os.path.abspath(imagePath)
imagePath = os.path.join(imagePath, 'documents', 'Images', 'MaterialXLogo.png')

image1.CreateInput("file", Sdf.ValueTypeNames.Asset).Set(imagePath)

standardSurface = UsdShade.Shader.Define(stage, material.GetPath().AppendPath('standardSurface1'))
standardSurface.CreateIdAttr('ND_standard_surface_surfaceshader')
standardSurface.CreateInput("base", Sdf.ValueTypeNames.Float).Set((1.))
# standardSurface.CreateInput("base_color", Sdf.ValueTypeNames.Color3f).Set((0.98, 0.01, 0.01))
standardSurface.CreateInput("base_color", Sdf.ValueTypeNames.Color3f).ConnectToSource(image1, 'out')

material.CreateSurfaceOutput().ConnectToSource(standardSurface, "out")

stage.Save()