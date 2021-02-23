//
// TM & (c) 2020 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved. See LICENSE.txt for license.
//

#include <MaterialXCodegen/OSL/OslGenerator.h>
#include <MaterialXCodegen/OSL/OslSyntax.h>
#include <MaterialXCodegen/OSL/OslContext.h>
#include <MaterialXCodegen/Fragments/Conditionals.h>
#include <MaterialXCodegen/Fragments/Convert.h>
#include <MaterialXCodegen/Fragments/Bsdfs.h>

#include <MaterialXRuntime/RtApi.h>

namespace MaterialX
{
namespace Codegen
{

OslGenerator::OslGenerator(const Context& context) :
    FragmentGenerator(context)
{
    // <ifgreater>
    registerFragmentClass(RtToken("IM_ifgreater_float_genosl"), IfGreaterFragment::create);
    registerFragmentClass(RtToken("IM_ifgreater_color3_genosl"), IfGreaterFragment::create);
    registerFragmentClass(RtToken("IM_ifgreater_color4_genosl"), IfGreaterFragment::create);
    registerFragmentClass(RtToken("IM_ifgreater_vector2_genosl"), IfGreaterFragment::create);
    registerFragmentClass(RtToken("IM_ifgreater_vector3_genosl"), IfGreaterFragment::create);
    registerFragmentClass(RtToken("IM_ifgreater_vector4_genosl"), IfGreaterFragment::create);
    registerFragmentClass(RtToken("IM_ifgreater_floatI_genosl"), IfGreaterFragment::create);
    registerFragmentClass(RtToken("IM_ifgreater_color3I_genosl"), IfGreaterFragment::create);
    registerFragmentClass(RtToken("IM_ifgreater_color4I_genosl"), IfGreaterFragment::create);
    registerFragmentClass(RtToken("IM_ifgreater_vector2I_genosl"), IfGreaterFragment::create);
    registerFragmentClass(RtToken("IM_ifgreater_vector3I_genosl"), IfGreaterFragment::create);
    registerFragmentClass(RtToken("IM_ifgreater_vector4I_genosl"), IfGreaterFragment::create);

    // <ifgreatereq>
    registerFragmentClass(RtToken("IM_ifgreatereq_float_genosl"), IfGreaterEqFragment::create);
    registerFragmentClass(RtToken("IM_ifgreatereq_color3_genosl"), IfGreaterEqFragment::create);
    registerFragmentClass(RtToken("IM_ifgreatereq_color4_genosl"), IfGreaterEqFragment::create);
    registerFragmentClass(RtToken("IM_ifgreatereq_vector2_genosl"), IfGreaterEqFragment::create);
    registerFragmentClass(RtToken("IM_ifgreatereq_vector3_genosl"), IfGreaterEqFragment::create);
    registerFragmentClass(RtToken("IM_ifgreatereq_vector4_genosl"), IfGreaterEqFragment::create);
    registerFragmentClass(RtToken("IM_ifgreatereq_floatI_genosl"), IfGreaterEqFragment::create);
    registerFragmentClass(RtToken("IM_ifgreatereq_color3I_genosl"), IfGreaterEqFragment::create);
    registerFragmentClass(RtToken("IM_ifgreatereq_color4I_genosl"), IfGreaterEqFragment::create);
    registerFragmentClass(RtToken("IM_ifgreatereq_vector2I_genosl"), IfGreaterEqFragment::create);
    registerFragmentClass(RtToken("IM_ifgreatereq_vector3I_genosl"), IfGreaterEqFragment::create);
    registerFragmentClass(RtToken("IM_ifgreatereq_vector4I_genosl"), IfGreaterEqFragment::create);

    // <ifequal>
    registerFragmentClass(RtToken("IM_ifequal_float_genosl"), IfEqualFragment::create);
    registerFragmentClass(RtToken("IM_ifequal_color3_genosl"), IfEqualFragment::create);
    registerFragmentClass(RtToken("IM_ifequal_color4_genosl"), IfEqualFragment::create);
    registerFragmentClass(RtToken("IM_ifequal_vector2_genosl"), IfEqualFragment::create);
    registerFragmentClass(RtToken("IM_ifequal_vector3_genosl"), IfEqualFragment::create);
    registerFragmentClass(RtToken("IM_ifequal_vector4_genosl"), IfEqualFragment::create);
    registerFragmentClass(RtToken("IM_ifequal_floatI_genosl"), IfEqualFragment::create);
    registerFragmentClass(RtToken("IM_ifequal_color3I_genosl"), IfEqualFragment::create);
    registerFragmentClass(RtToken("IM_ifequal_color4I_genosl"), IfEqualFragment::create);
    registerFragmentClass(RtToken("IM_ifequal_vector2I_genosl"), IfEqualFragment::create);
    registerFragmentClass(RtToken("IM_ifequal_vector3I_genosl"), IfEqualFragment::create);
    registerFragmentClass(RtToken("IM_ifequal_vector4I_genosl"), IfEqualFragment::create);
    registerFragmentClass(RtToken("IM_ifequal_floatB_genosl"), IfEqualFragment::create);
    registerFragmentClass(RtToken("IM_ifequal_color3B_genosl"), IfEqualFragment::create);
    registerFragmentClass(RtToken("IM_ifequal_color4B_genosl"), IfEqualFragment::create);
    registerFragmentClass(RtToken("IM_ifequal_vector2B_genosl"), IfEqualFragment::create);
    registerFragmentClass(RtToken("IM_ifequal_vector3B_genosl"), IfEqualFragment::create);
    registerFragmentClass(RtToken("IM_ifequal_vector4B_genosl"), IfEqualFragment::create);

    // <switch>
    registerFragmentClass(RtToken("IM_switch_float_genosl"), SwitchFragment::create);
    registerFragmentClass(RtToken("IM_switch_color3_genosl"), SwitchFragment::create);
    registerFragmentClass(RtToken("IM_switch_color4_genosl"), SwitchFragment::create);
    registerFragmentClass(RtToken("IM_switch_vector2_genosl"), SwitchFragment::create);
    registerFragmentClass(RtToken("IM_switch_vector3_genosl"), SwitchFragment::create);
    registerFragmentClass(RtToken("IM_switch_vector4_genosl"), SwitchFragment::create);
    registerFragmentClass(RtToken("IM_switch_floatI_genosl"), SwitchFragment::create);
    registerFragmentClass(RtToken("IM_switch_color3I_genosl"), SwitchFragment::create);
    registerFragmentClass(RtToken("IM_switch_color4I_genosl"), SwitchFragment::create);
    registerFragmentClass(RtToken("IM_switch_vector2I_genosl"), SwitchFragment::create);
    registerFragmentClass(RtToken("IM_switch_vector3I_genosl"), SwitchFragment::create);
    registerFragmentClass(RtToken("IM_switch_vector4I_genosl"), SwitchFragment::create);

    // <convert>
    registerFragmentClass(RtToken("IM_convert_float_color3_genosl"), ConvertFragment::create);
    registerFragmentClass(RtToken("IM_convert_float_color4_genosl"), ConvertFragment::create);
    registerFragmentClass(RtToken("IM_convert_float_vector2_genosl"), ConvertFragment::create);
    registerFragmentClass(RtToken("IM_convert_float_vector3_genosl"), ConvertFragment::create);
    registerFragmentClass(RtToken("IM_convert_float_vector4_genosl"), ConvertFragment::create);
    registerFragmentClass(RtToken("IM_convert_vector2_vector3_genosl"), ConvertFragment::create);
    registerFragmentClass(RtToken("IM_convert_vector3_vector2_genosl"), ConvertFragment::create);
    registerFragmentClass(RtToken("IM_convert_vector3_color3_genosl"), ConvertFragment::create);
    registerFragmentClass(RtToken("IM_convert_vector3_vector4_genosl"), ConvertFragment::create);
    registerFragmentClass(RtToken("IM_convert_vector4_vector3_genosl"), ConvertFragment::create);
    registerFragmentClass(RtToken("IM_convert_vector4_color4_genosl"), ConvertFragment::create);
    registerFragmentClass(RtToken("IM_convert_color3_vector3_genosl"), ConvertFragment::create);
    registerFragmentClass(RtToken("IM_convert_color4_vector4_genosl"), ConvertFragment::create);
    registerFragmentClass(RtToken("IM_convert_color3_color4_genosl"), ConvertFragment::create);
    registerFragmentClass(RtToken("IM_convert_color4_color3_genosl"), ConvertFragment::create);
    registerFragmentClass(RtToken("IM_convert_boolean_float_genosl"), ConvertFragment::create);
    registerFragmentClass(RtToken("IM_convert_integer_float_genosl"), ConvertFragment::create);

    // <swizzle> : float
    registerFragmentClass(RtToken("IM_swizzle_float_color3_genosl"), SwizzleFragment::create);
    registerFragmentClass(RtToken("IM_swizzle_float_color4_genosl"), SwizzleFragment::create);
    registerFragmentClass(RtToken("IM_swizzle_float_vector2_genosl"), SwizzleFragment::create);
    registerFragmentClass(RtToken("IM_swizzle_float_vector3_genosl"), SwizzleFragment::create);
    registerFragmentClass(RtToken("IM_swizzle_float_vector4_genosl"), SwizzleFragment::create);
    // <swizzle> : color3
    registerFragmentClass(RtToken("IM_swizzle_color3_float_genosl"), SwizzleFragment::create);
    registerFragmentClass(RtToken("IM_swizzle_color3_color3_genosl"), SwizzleFragment::create);
    registerFragmentClass(RtToken("IM_swizzle_color3_color4_genosl"), SwizzleFragment::create);
    registerFragmentClass(RtToken("IM_swizzle_color3_vector2_genosl"), SwizzleFragment::create);
    registerFragmentClass(RtToken("IM_swizzle_color3_vector3_genosl"), SwizzleFragment::create);
    registerFragmentClass(RtToken("IM_swizzle_color3_vector4_genosl"), SwizzleFragment::create);
    // <swizzle> : color4
    registerFragmentClass(RtToken("IM_swizzle_color4_float_genosl"), SwizzleFragment::create);
    registerFragmentClass(RtToken("IM_swizzle_color4_color3_genosl"), SwizzleFragment::create);
    registerFragmentClass(RtToken("IM_swizzle_color4_color4_genosl"), SwizzleFragment::create);
    registerFragmentClass(RtToken("IM_swizzle_color4_vector2_genosl"), SwizzleFragment::create);
    registerFragmentClass(RtToken("IM_swizzle_color4_vector3_genosl"), SwizzleFragment::create);
    registerFragmentClass(RtToken("IM_swizzle_color4_vector4_genosl"), SwizzleFragment::create);
    // <swizzle> : vector2
    registerFragmentClass(RtToken("IM_swizzle_vector2_float_genosl"), SwizzleFragment::create);
    registerFragmentClass(RtToken("IM_swizzle_vector2_color3_genosl"), SwizzleFragment::create);
    registerFragmentClass(RtToken("IM_swizzle_vector2_color4_genosl"), SwizzleFragment::create);
    registerFragmentClass(RtToken("IM_swizzle_vector2_vector2_genosl"), SwizzleFragment::create);
    registerFragmentClass(RtToken("IM_swizzle_vector2_vector3_genosl"), SwizzleFragment::create);
    registerFragmentClass(RtToken("IM_swizzle_vector2_vector4_genosl"), SwizzleFragment::create);
    // <swizzle> : vector3
    registerFragmentClass(RtToken("IM_swizzle_vector3_float_genosl"), SwizzleFragment::create);
    registerFragmentClass(RtToken("IM_swizzle_vector3_color3_genosl"), SwizzleFragment::create);
    registerFragmentClass(RtToken("IM_swizzle_vector3_color4_genosl"), SwizzleFragment::create);
    registerFragmentClass(RtToken("IM_swizzle_vector3_vector2_genosl"), SwizzleFragment::create);
    registerFragmentClass(RtToken("IM_swizzle_vector3_vector3_genosl"), SwizzleFragment::create);
    registerFragmentClass(RtToken("IM_swizzle_vector3_vector4_genosl"), SwizzleFragment::create);
    // <swizzle> : vector4
    registerFragmentClass(RtToken("IM_swizzle_vector4_float_genosl"), SwizzleFragment::create);
    registerFragmentClass(RtToken("IM_swizzle_vector4_color3_genosl"), SwizzleFragment::create);
    registerFragmentClass(RtToken("IM_swizzle_vector4_color4_genosl"), SwizzleFragment::create);
    registerFragmentClass(RtToken("IM_swizzle_vector4_vector2_genosl"), SwizzleFragment::create);
    registerFragmentClass(RtToken("IM_swizzle_vector4_vector3_genosl"), SwizzleFragment::create);
    registerFragmentClass(RtToken("IM_swizzle_vector4_vector4_genosl"), SwizzleFragment::create);

    // <sheen_bsdf>
    registerFragmentClass(RtToken("IM_sheen_bsdf_genosl"), SheenBsdfFragment::create);

    registerFragmentClass(RtToken("IM_layer_bsdf_genosl"), ConvertFragment::create);
    registerFragmentClass(RtToken("IM_thin_film_bsdf_genosl"), ConvertFragment::create);
}

FragmentGeneratorPtr OslGenerator::create(const Context& context)
{
    return FragmentGeneratorPtr(new OslGenerator(context));
}

} // namespace Codegen
} // namespace MaterialX
