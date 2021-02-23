//
// TM & (c) 2017 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved.  See LICENSE.txt for license.
//

#ifndef MATERIALX_CODEGEN_COLOR_MANAGEMENT_H
#define MATERIALX_CODEGEN_COLOR_MANAGEMENT_H

/// @file
/// Color management classes

#include <MaterialXCodegen/Library.h>
#include <MaterialXCodegen/Fragment.h>

#include <MaterialXRuntime/RtToken.h>

#include <MaterialXFormat/File.h>

namespace MaterialX
{
namespace Codegen
{

/// @struct ColorSpaceTransform
/// Structure that represents color space transform information
struct ColorSpaceTransform
{
    RtToken type;
    RtToken sourceSpace;
    RtToken targetSpace;

    ColorSpaceTransform();
    ColorSpaceTransform(const RtToken& type, const RtToken& sourceSpace, const RtToken& targetSpace);
};

/// @class ColorManagementSystem
/// Abstract base class for color management systems
class ColorManagementSystem
{
  public:
    /// Destructor.
    virtual ~ColorManagementSystem() {}

    /// Return the ColorManagementSystem name.
    virtual const RtToken& getName() const = 0;

    /// Return a fragment for the given color space transform,
    /// or nullptr if this transform is not supported by the system.
    virtual FragmentPtr createFragment(const ColorSpaceTransform& transform) const = 0;

    template<class T>
    T* asA()
    {
        return static_cast<T*>(this);
    }

    template<class T>
    const T* asA() const
    {
        return static_cast<T*>(this);
    }

  protected:
    /// Protected constructor.
    ColorManagementSystem() {};
};

/// A shared pointer to a ColorManagementSystem
using ColorManagementSystemPtr = RtSharedPtr<class ColorManagementSystem>;


/// @class DefaultColorManagementSystem
/// Class implementing a simple color management system
/// support a set of commonly used color transformations.
class DefaultColorManagementSystem : public ColorManagementSystem
{
public:
    virtual ~DefaultColorManagementSystem();

    /// Create a new DefaultColorManagementSystem for the given target.
    static ColorManagementSystemPtr create(const Context& context);

    /// Return the ColorManagementSystem name.
    const RtToken& getName() const override;

    /// Returns an implementation for a given transform
    FragmentPtr createFragment(const ColorSpaceTransform& transform) const override;

    /// Load color transform implementations from a MaterialX file.
    void loadImplementations(const FilePath& file);

protected:
    /// Protected constructor
    DefaultColorManagementSystem(const Context& context);

private:
    const Context& _context;
    RtTokenMap<FragmentPtr> _fragments;
    vector<string*> _sourceCode;
};

} // namespace Codegen
} // namespace MaterialX

#endif
