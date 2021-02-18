//
// TM & (c) 2021 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved. See LICENSE.txt for license.
//

#include <MaterialXCodegen/Attribute.h>
#include <MaterialXCodegen/Fragment.h>

namespace MaterialX
{
namespace Codegen
{

string Attribute::getLongName() const
{
    return _parent->getName().str() + "_" + _name.str();
}

} // namespace Codegen
} // namespace MaterialX
