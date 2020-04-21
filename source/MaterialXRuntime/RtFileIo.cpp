//
// TM & (c) 2019 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved.  See LICENSE.txt for license.
//

#include <MaterialXRuntime/RtFileIo.h>
#include <MaterialXRuntime/RtObject.h>
#include <MaterialXRuntime/RtNodeDef.h>
#include <MaterialXRuntime/RtNode.h>
#include <MaterialXRuntime/RtNodeGraph.h>
#include <MaterialXRuntime/RtBackdrop.h>
#include <MaterialXRuntime/RtLook.h>
#include <MaterialXRuntime/RtCollection.h>
#include <MaterialXRuntime/RtGeneric.h>
#include <MaterialXRuntime/RtTypeDef.h>
#include <MaterialXRuntime/RtTraversal.h>
#include <MaterialXRuntime/RtApi.h>

#include <MaterialXRuntime/Private/PvtStage.h>

#include <MaterialXCore/Types.h>

#include <MaterialXGenShader/Util.h>

#ifdef MATERIALX_BUILD_USD
#include <pxr/base/tf/token.h>
#include <pxr/usd/usd/stage.h>
#include <pxr/usd/usd/primRange.h>
#include <pxr/usd/usd/prim.h>
#include <pxr/usd/usdShade/material.h>
#include <pxr/usd/usdShade/shader.h>
#include <pxr/usd/usdShade/connectableAPI.h>
#endif

#include <map>
#include <sstream>

namespace MaterialX
{

namespace
{
    // Lists of known metadata which are handled explicitly by import/export.
    static const RtTokenSet nodedefMetadata     = { RtToken("name"), RtToken("type"), RtToken("node") };
    static const RtTokenSet attrMetadata        = { RtToken("name"), RtToken("type"), RtToken("value"), RtToken("nodename"), RtToken("output"), RtToken("colorspace"), RtToken("unit"), RtToken("unittype") };
    static const RtTokenSet nodeMetadata        = { RtToken("name"), RtToken("type"), RtToken("node") };
    static const RtTokenSet nodegraphMetadata   = { RtToken("name") };
    static const RtTokenSet genericMetadata     = { RtToken("name"), RtToken("kind") };
    static const RtTokenSet stageMetadata       = {};

    static const RtToken DEFAULT_OUTPUT("out");
    static const RtToken OUTPUT_ELEMENT_PREFIX("OUT_");
    static const RtToken MULTIOUTPUT("multioutput");

#ifdef MATERIALX_BUILD_USD
    // Usd to MaterialX types
    const std::map<pxr::TfToken, RtToken> usdToMxTypes {
        { pxr::TfToken{"float"}, RtToken{"float"} },
        { pxr::TfToken{"asset"}, RtToken{"filename"} },
        { pxr::TfToken{"color3f"}, RtToken{"color3"} },
        { pxr::TfToken{"vector3f"}, RtToken{"vector3"} },
    };
#endif

    class PvtRenamingMapper {
        typedef RtTokenMap<RtToken> TokenToToken;
        typedef std::map<PvtPrim*, TokenToToken> PerPrimMap;

        PerPrimMap _map;
    public:
        void addMapping(PvtPrim* parent, const RtToken& originalName, const RtToken& finalName) {
            if (originalName != finalName) {
                _map[parent][originalName] = finalName;
            }
        }

        const RtToken& getFinalName(PvtPrim* parent, const RtToken& originalName) const {
            PerPrimMap::const_iterator primTarget = _map.find(parent);
            if (primTarget != _map.cend()) {
                const TokenToToken& nameMap = primTarget->second;
                TokenToToken::const_iterator nameTarget = nameMap.find(originalName);
                if (nameTarget != nameMap.cend()) {
                    return nameTarget->second;
                }
            }
            return originalName;
        }
    };

    PvtPrim* findPrimOrThrow(const RtToken& name, PvtPrim* parent, const PvtRenamingMapper& mapper)
    {
        PvtPrim* prim = parent->getChild(mapper.getFinalName(parent, name));
        if (!prim)
        {
            throw ExceptionRuntimeError("Can't find node '" + name.str() + "' in '" + parent->getName().str() + "'");
        }
        return prim;
    }

    PvtInput* findInputOrThrow(const RtToken& name, PvtPrim* prim)
    {
        PvtInput* input = prim->getInput(name);
        if (!input)
        {
            throw ExceptionRuntimeError("Node '" + prim->getName().str() + "' has no input named '" + name.str() + "'");
        }
        return input;
    }

    PvtOutput* findOutputOrThrow(const RtToken& name, PvtPrim* prim)
    {
        PvtOutput* output = prim->getOutput(name);
        if (!output)
        {
            throw ExceptionRuntimeError("Node '" + prim->getName().str() + "' has no output named '" + name.str() + "'");
        }
        return output;
    }

    void readMetadata(const ElementPtr src, PvtObject* dest, const RtTokenSet& ignoreList)
    {
        // Read in all metadata so we can export the element again
        // without loosing data.
        for (const string& name : src->getAttributeNames())
        {
            const RtToken mdName(name);
            if (!ignoreList.count(mdName))
            {
                // Store all custom attributes as string tokens.
                RtTypedValue* md = dest->addMetadata(mdName, RtType::TOKEN);
                md->getValue().asToken() = src->getAttribute(name);
            }
        }
    }

    void writeMetadata(const PvtObject* src, ElementPtr dest, const RtTokenSet& ignoreList)
    {
        for (const RtToken name : src->getMetadataOrder())
        {
            if (ignoreList.count(name) ||
                (name.str().size() > 0 && name.str().at(0) == '_')) // Metadata with "_" prefix are private
            {
                continue;
            }
            const RtTypedValue* md = src->getMetadata(name);
            dest->setAttribute(name.str(), md->getValueString());
        }
    }

    template<class T>
    void createInterface(const ElementPtr src, T schema)
    {
        for (auto elem : src->getChildrenOfType<ValueElement>())
        {
            const RtToken attrName(elem->getName());
            const RtToken attrType(elem->getType());

            RtAttribute attr;
            if (elem->isA<Output>())
            {
                attr = schema.createOutput(attrName, attrType);
            }
            else if (elem->isA<Input>())
            {
                attr = schema.createInput(attrName, attrType);
            }
            else
            {
                attr = schema.createInput(attrName, attrType, RtAttrFlag::UNIFORM);
            }

            const string& valueStr = elem->getValueString();
            if (!valueStr.empty())
            {
                RtValue::fromString(attrType, valueStr, attr.getValue());
            }
            const string& colorSpace = elem->getColorSpace();
            if (!colorSpace.empty())
            {
                attr.setColorSpace(RtToken(elem->getColorSpace()));
            }
            const string& unitStr = elem->getUnit();
            if (!unitStr.empty())
            {
                attr.setUnit(RtToken(unitStr));
            }
            const string& unitTypeStr = elem->getUnitType();
            if (!unitTypeStr.empty())
            {
                attr.setUnitType(RtToken(unitTypeStr));
            }

            readMetadata(elem, PvtObject::ptr<PvtObject>(attr), attrMetadata);
        }
    }

    void createNodeConnections(const vector<NodePtr>& nodeElements, PvtPrim* parent, const PvtRenamingMapper&mapper)
    {
        for (const NodePtr& nodeElem : nodeElements)
        {
            PvtPrim* node = findPrimOrThrow(RtToken(nodeElem->getName()), parent, mapper);
            for (const InputPtr& elemInput : nodeElem->getInputs())
            {
                PvtInput* input = findInputOrThrow(RtToken(elemInput->getName()), node);
                string connectedNodeName = elemInput->getNodeName();
                if (connectedNodeName.empty())
                {
                    connectedNodeName = elemInput->getNodeGraphName();
                }
                if (!connectedNodeName.empty())
                {
                    PvtPrim* connectedNode = findPrimOrThrow(RtToken(connectedNodeName), parent, mapper);
                    const RtToken outputName(elemInput->getOutputString());
                    PvtOutput* output = findOutputOrThrow(outputName != EMPTY_TOKEN ? outputName : PvtAttribute::DEFAULT_OUTPUT_NAME, connectedNode);
                    output->connect(input);
                }
            }
        }
    }

    PvtPrim* readNodeDef(const NodeDefPtr& src, PvtStage* stage)
    {
        const RtToken name(src->getName());
        PvtPrim* prim = stage->createPrim(stage->getPath(), name, RtNodeDef::typeName());

        const RtToken nodeName(src->getNodeString());
        RtNodeDef nodedef(prim->hnd());
        nodedef.setNode(nodeName);

        readMetadata(src, prim, nodedefMetadata);

        // Create the interface.
        createInterface(src, nodedef);

        return prim;
    }

    bool matchingSignature(const PvtPrim* prim, const RtToken& nodeType, const vector<ValueElementPtr>& nodePorts)
    {
        if (nodeType != MULTIOUTPUT)
        {
            // For single output nodes we can match the output directly.
            PvtOutput* out = prim->getOutput(PvtAttribute::DEFAULT_OUTPUT_NAME);
            if (!out || out->getType() != nodeType)
            {
                return false;
            }
        }

        // Check all ports.
        // TODO: Do we need to match port type as well (input/output/parameter)?
        for (const ValueElementPtr& nodePort : nodePorts)
        {
            const PvtAttribute* attr = prim->getAttribute(RtToken(nodePort->getName()));
            if (!attr || attr->getType().str() != nodePort->getType())
            {
                return false;
            }
        }
        return true;
    }

    RtToken resolveNodeDefName(const NodePtr& node)
    {
        // First, see if a nodedef is specified on the node.
        const string& nodedefString = node->getNodeDefString();
        if (!nodedefString.empty())
        {
            return RtToken(nodedefString);
        }

        // Second, try resolving a nodedef from content in the current document.
        NodeDefPtr srcNodedef = node->getNodeDef();
        if (srcNodedef)
        {
            const RtToken nodedefName(srcNodedef->getName());
            return nodedefName;
        }

        // Third, try resolving among existing registered master prim nodedefs.
        const RtToken nodeName(node->getCategory());
        const RtToken nodeType(node->getType());
        const vector<ValueElementPtr> nodePorts = node->getActiveValueElements();
        RtSchemaPredicate<RtNodeDef> nodedefFilter;
        for (RtPrim masterPrim : RtApi::get().getMasterPrims(nodedefFilter))
        {
            RtNodeDef candidate(masterPrim);
            if (candidate.getNode() == nodeName &&
                matchingSignature(PvtObject::ptr<PvtPrim>(masterPrim), nodeType, nodePorts))
            {
                return candidate.getName();
            }
        }

        return EMPTY_TOKEN;
    }

#ifdef MATERIALX_BUILD_USD

    PvtPrim* readUsdShader(const pxr::UsdPrim& src, PvtStage* stage)
    {
        pxr::UsdShadeShader shaderAPI(src);
        pxr::TfToken id;
        shaderAPI.GetShaderId(&id);

        const RtToken nodedefName(id.GetText());
        // TODO: check nodedefName is valid

        const RtToken nodeName(src.GetName());
        PvtPrim* node = stage->createPrim(PvtPath(nodeName), nodedefName);
        return node;
    }
#endif

    PvtPrim* readNode(const NodePtr& src, PvtPrim* parent, PvtStage* stage, PvtRenamingMapper& mapper)
    {
        const RtToken nodedefName = resolveNodeDefName(src);
        if (nodedefName == EMPTY_TOKEN)
        {
            throw ExceptionRuntimeError("No matching nodedef was found for node '" + src->getName() + "'");
        }

        const RtToken nodeName(src->getName());
        PvtPrim* node = stage->createPrim(parent->getPath(), nodeName, nodedefName);
        mapper.addMapping(parent, nodeName, node->getName());

        readMetadata(src, node, attrMetadata);

        // Copy input values.
        for (auto elem : src->getChildrenOfType<ValueElement>())
        {
            if (elem->isA<Output>())
            {
                continue;
            }
            const RtToken portName(elem->getName());
            PvtAttribute* input = node->getInput(portName);
            if (!input)
            {
                throw ExceptionRuntimeError("No input named '" + elem->getName() + "' was found on runtime node '" + node->getName().str() + "'");
            }
            const string& valueStr = elem->getValueString();
            if (!valueStr.empty())
            {
                const RtToken portType(elem->getType());
                RtValue::fromString(portType, valueStr, input->getValue());
            }
            const string& colorSpace = elem->getColorSpace();
            if (!colorSpace.empty())
            {
                input->setColorSpace(RtToken(elem->getColorSpace()));
            }
            const string& unitStr = elem->getUnit();
            if (!unitStr.empty())
            {
                input->setUnit(RtToken(unitStr));
            }
            const string& unitTypeStr = elem->getUnitType();
            if (!unitTypeStr.empty())
            {
                input->setUnitType(RtToken(unitTypeStr));
            }
        }

        return node;
    }

#ifdef MATERIALX_BUILD_USD

void traverseUsdShader(const pxr::UsdShadeShader& usdShader, PvtPrim* mxShader, PvtStage* mxStage)
{
    for(const auto& usdInput : usdShader.GetInputs())
    {        
        PvtInput* mxInput = mxShader->getInput(RtToken(usdInput.GetBaseName().GetText()));
        if(!mxInput)
        {
            throw ExceptionRuntimeError("No input named '" + usdInput.GetBaseName().GetString() + "' was found on runtime node '" + mxShader->getName().str() + "'");
        }



        pxr::UsdShadeConnectableAPI connectableAPI;
        pxr::TfToken usdSourceName;
        pxr::UsdShadeAttributeType sourceType;

        if(usdInput.GetConnectedSource(&connectableAPI, &usdSourceName, &sourceType))
        {
            PvtPrim* mxSourceShader = readUsdShader(connectableAPI.GetPrim(), mxStage);
            if(mxSourceShader)
            {
                const RtToken mxSourceName(usdSourceName.GetText());
                PvtOutput* mxSourceOutput = mxSourceShader->getOutput(mxSourceName);
                if (!mxSourceOutput)
                {
                    throw ExceptionRuntimeError("No output named '" + mxSourceName.str() + "' was found on runtime node '" + mxSourceShader->getName().str() + "'");
                }

                mxSourceOutput->connect(mxInput);
        
                traverseUsdShader(pxr::UsdShadeShader(connectableAPI.GetPrim()), mxSourceShader, mxStage);
            }
        }
        else
        {
            const pxr::TfToken usdType = usdInput.GetTypeName().GetAsToken();
            auto search = usdToMxTypes.find(usdType);
            if (search != usdToMxTypes.end()) 
            {
                if(search->second == RtToken{"float"})
                {
                    float usdValue;
                    usdInput.Get(&usdValue);
                    mxInput->setValue(RtValue(usdValue));
                }
                else if(search->second == RtToken{"filename"})
                {
                    pxr::VtValue usdValue;
                    usdInput.Get(&usdValue);
                    RtValue::fromString(
                        RtToken{"filename"}, 
                        usdValue.UncheckedGet<pxr::TfToken>().GetString(),
                        mxInput->getValue());
                }
            }            
        }
    }
}
void traverseUsdMaterial(const pxr::UsdShadeMaterial& usdMaterial, PvtPrim* mxMaterial, PvtStage* mxStage)
{
    // In USD, material outputs are connected to shader outputs
    for(const auto& usdOutput : usdMaterial.GetOutputs())
    {
        PvtInput* mxInput;
        if(usdOutput.GetBaseName() == pxr::TfToken("surface"))
        {
            mxInput = mxMaterial->getInput(RtToken("surfaceshader"));
        }
        else if(usdOutput.GetBaseName() == pxr::TfToken("displacement"))
        {
            mxInput = mxMaterial->getInput(RtToken("displacementshader"));
        }
        else
        {
            continue;
        }

        pxr::UsdShadeConnectableAPI connectableAPI;
        pxr::TfToken usdSourceName;
        pxr::UsdShadeAttributeType sourceType;

        if(usdOutput.GetConnectedSource(&connectableAPI, &usdSourceName, &sourceType))
        {
            pxr::UsdShadeShader usdSourceShader(connectableAPI.GetPrim());
            PvtPrim* mxSourceShader = readUsdShader(connectableAPI.GetPrim(), mxStage);
            if(mxSourceShader)
            {
                const RtToken mxSourceName(usdSourceName.GetText());
                PvtOutput* mxSourceOutput = mxSourceShader->getOutput(mxSourceName);
                if (!mxSourceOutput)
                {
                    throw ExceptionRuntimeError("No output named '" + mxSourceName.str() + "' was found on runtime node '" + mxSourceShader->getName().str() + "'");
                }

                mxSourceOutput->connect(mxInput);
        
                traverseUsdShader(pxr::UsdShadeShader(connectableAPI.GetPrim()), mxSourceShader, mxStage);
            }          
        }
    }
}

#endif

#ifdef MATERIALX_BUILD_USD
    PvtPrim* readUsdMaterial(const pxr::UsdPrim& src, PvtPrim* parent, PvtStage* mxStage)
    {
        pxr::UsdShadeMaterial usdMaterial(src);
        RtToken materialTypeName;
        if(usdMaterial.GetSurfaceOutput().HasConnectedSource() ||
           usdMaterial.GetDisplacementOutput().HasConnectedSource())
        {
            materialTypeName = "ND_surfacematerial";
        }
        else if(usdMaterial.GetVolumeOutput().HasConnectedSource())
        {
            materialTypeName = "ND_volumematerial";
        }
        else
        {
            return nullptr;
        }

        const RtToken materialName(src.GetName());
        PvtPrim* mxMaterial = mxStage->createPrim(parent->getPath(), materialName, materialTypeName);

        // TODO: read metadata of the USD Material?

        traverseUsdMaterial(usdMaterial, mxMaterial, mxStage);

        return mxMaterial;
    }
#endif

    PvtPrim* readNodeGraph(const NodeGraphPtr& src, PvtPrim* parent, PvtStage* stage, PvtRenamingMapper& mapper)
    {
        const RtToken nodegraphName(src->getName());

        PvtPrim* nodegraph = stage->createPrim(parent->getPath(), nodegraphName, RtNodeGraph::typeName());
        mapper.addMapping(parent, nodegraphName, nodegraph->getName());
        RtNodeGraph schema(nodegraph->hnd());

        readMetadata(src, nodegraph, nodegraphMetadata);

        // Create the interface either from a nodedef if given
        // otherwise from the graph itself.
        const NodeDefPtr srcNodeDef = src->getNodeDef();
        if (srcNodeDef)
        {
            createInterface(srcNodeDef, schema);
        }
        else
        {
            createInterface(src, schema);
        }

        // Create all nodes and connection between node inputs and internal graph sockets.
        for (auto child : src->getChildren())
        {
            NodePtr srcNnode = child->asA<Node>();
            if (srcNnode)
            {
                PvtPrim* node = readNode(srcNnode, nodegraph, stage, mapper);

                // Check for connections to the internal graph sockets
                for (auto elem : srcNnode->getChildrenOfType<ValueElement>())
                {
                    if (elem->isA<Output>())
                    {
                        continue;
                    }
                    const string& interfaceName = elem->getInterfaceName();
                    if (!interfaceName.empty())
                    {
                        const RtToken inputName(elem->getName());
                        const RtToken socketName(interfaceName);
                        RtOutput socket = schema.getInputSocket(socketName);
                        if (!socket)
                        {
                            const RtToken inputType(elem->getType());
                            RtInput input = schema.createInput(socketName, inputType);
                            socket = schema.getInputSocket(input.getName());
                        }

                        RtInput input(findInputOrThrow(inputName, node)->hnd());
                        socket.connect(input);
                    }
                }
            }
        }

        // Create connections between all nodes.
        createNodeConnections(src->getNodes(), nodegraph, mapper);

        // Create connections between node outputs and internal graph sockets.
        for (const OutputPtr& elem : src->getOutputs())
        {
            const string& connectedNodeName = elem->getNodeName();
            if (!connectedNodeName.empty())
            {
                RtInput socket = schema.getOutputSocket(RtToken(elem->getName()));
                if (!socket)
                {
                    PvtPath path(parent->getPath());
                    path.push(nodegraphName);
                    throw ExceptionRuntimeError("Output '" + elem->getName() + "' does not match an internal output socket on the nodegraph '" + path.asString() + "'");
                }

                PvtPrim* connectedNode = findPrimOrThrow(RtToken(connectedNodeName), nodegraph, mapper);

                const RtToken outputName(elem->getOutputString());
                RtOutput output(findOutputOrThrow(outputName != EMPTY_TOKEN ? outputName : PvtAttribute::DEFAULT_OUTPUT_NAME, connectedNode)->hnd());
                output.connect(socket);
            }
        }

        return nodegraph;
    }

    PvtPrim* readGenericPrim(const ElementPtr& src, PvtPrim* parent, PvtStage* stage, PvtRenamingMapper& mapper)
    {
        const RtToken name(src->getName());
        const RtToken category(src->getCategory());

        PvtPrim* prim = stage->createPrim(parent->getPath(), name, RtGeneric::typeName());
        mapper.addMapping(parent, name, prim->getName());
        RtGeneric generic(prim->hnd());
        generic.setKind(category);

        readMetadata(src, prim, genericMetadata);

        for (auto child : src->getChildren())
        {
            readGenericPrim(child, prim, stage, mapper);
        }

        return prim;
    }

    // Note that this function reads in a single collection. After all required collections
    // have been read in, the createCollectionConnections() function can be called
    // to create collection inclusion connections.
    PvtPrim* readCollection(const CollectionPtr& src, PvtPrim* parent, PvtStage* stage, PvtRenamingMapper& mapper)
    {
        const RtToken name(src->getName());

        PvtPrim* collectionPrim = stage->createPrim(parent->getPath(), name, RtCollection::typeName());
        mapper.addMapping(parent, name, collectionPrim->getName());
        RtCollection collection(collectionPrim->hnd());
        collection.getIncludeGeom().setValueString(src->getIncludeGeom());
        collection.getExcludeGeom().setValueString(src->getExcludeGeom());

        return collectionPrim;
    }

    // Create collection include connections assuming that all referenced
    // looks exist.
    void makeCollectionIncludeConnections(const vector<CollectionPtr>& collectionElements, PvtPrim* parent, const PvtRenamingMapper& mapper)
    {
        for (const CollectionPtr& colElement : collectionElements)
        {
            PvtPrim* parentCollection = findPrimOrThrow(RtToken(colElement->getName()), parent, mapper);
            for (const CollectionPtr& includeCollection : colElement->getIncludeCollections())
            {
                PvtPrim* childCollection = findPrimOrThrow(RtToken(includeCollection->getName()), parent, mapper);
                RtCollection rtCollection(parentCollection->hnd());
                rtCollection.addCollection(childCollection->hnd());
            }
        }
    }

    // Note that this function reads in a single look. After all required looks have been
    // read in then createLookConnections() can be called to create look inheritance
    // connections.
    PvtPrim* readLook(const LookPtr& src, PvtPrim* parent, PvtStage* stage, PvtRenamingMapper& mapper)
    {
        const RtToken name(src->getName());

        PvtPrim* lookPrim = stage->createPrim(parent->getPath(), name, RtLook::typeName());
        mapper.addMapping(parent, name, lookPrim->getName());
        RtLook look(lookPrim->hnd());

        // Create material assignments
        for (const MaterialAssignPtr matAssign : src->getMaterialAssigns())
        {
            const RtToken matAssignName(matAssign->getName());
            PvtPrim* assignPrim = stage->createPrim(parent->getPath(), matAssignName, RtMaterialAssign::typeName());
            mapper.addMapping(parent, matAssignName, assignPrim->getName());
            RtMaterialAssign rtMatAssign(assignPrim->hnd());
            
            if (!matAssign->getCollectionString().empty()) {
                PvtPrim* collection = findPrimOrThrow(RtToken(matAssign->getCollectionString()), parent, mapper);
                rtMatAssign.getCollection().addTarget(collection->hnd());
            }

            if (!matAssign->getMaterial().empty()) {
                PvtPrim* material = findPrimOrThrow(RtToken(matAssign->getMaterial()), parent, mapper);
                rtMatAssign.getMaterial().addTarget(material->hnd());
            }

            if (matAssign->hasAttribute(MaterialAssign::EXCLUSIVE_ATTRIBUTE)) {
                rtMatAssign.getExclusive().getValue().asBool() = matAssign->getExclusive();
            } else {
                rtMatAssign.getExclusive().getValue().asBool() = true; // default
            }

            rtMatAssign.getGeom().getValue().asString() = matAssign->getActiveGeom();

            look.getMaterialAssigns().addTarget(assignPrim->hnd());
        }
        return lookPrim;
    }

    // Create look inheritance connections assuming that all referenced
    // looks exist.
    void makeLookInheritConnections(const vector<LookPtr>& lookElements, PvtPrim* parent, const PvtRenamingMapper& mapper)
    {
        for (const LookPtr& lookElem : lookElements)
        {
            PvtPrim* childLook = findPrimOrThrow(RtToken(lookElem->getName()), parent, mapper);
            const string& inheritString = lookElem->getInheritString();
            if (!inheritString.empty())
            {
                PvtPrim* parentLook = findPrimOrThrow(RtToken(inheritString), parent, mapper);
                RtLook rtLook(childLook->hnd());
                rtLook.getInherit().addTarget(parentLook->hnd());
            }
        }
    }

    // Read in a look group. This assumes that all referenced looks have
    // already been created.
    PvtPrim* readLookGroup(const LookGroupPtr& src, PvtPrim* parent, PvtStage* stage, PvtRenamingMapper& mapper)
    {
        const string LIST_SEPARATOR(",");

        const RtToken name(src->getName());
        PvtPrim* prim = stage->createPrim(parent->getPath(), name, RtLookGroup::typeName());
        mapper.addMapping(parent, name, prim->getName());
        RtLookGroup lookGroup(prim->hnd());

        // Link to looks
        const string& lookNamesString = src->getLooks();
        StringVec lookNamesList  = splitString(lookNamesString, LIST_SEPARATOR);
        for (auto lookName : lookNamesList)
        {
            if (!lookName.empty())
            {
                PvtPrim* lookPrim = findPrimOrThrow(RtToken(lookName), parent, mapper);
                lookGroup.addLook(lookPrim->hnd());
            }
        }
        const string& activeLook = src->getActiveLook();
        lookGroup.getActiveLook().setValueString(activeLook);

        return prim;
    }

    // Read in all look information from a document. Collections, looks and
    // look groups are read in first. Then relationship linkages are made.
    void readLookInformation(const DocumentPtr& doc, PvtStage* stage, const RtReadOptions* readOptions, PvtRenamingMapper& mapper)
    {
        RtReadOptions::ReadFilter filter = readOptions ? readOptions->readFilter : nullptr;

        PvtPrim* rootPrim = stage->getRootPrim();

        // Read collections
        for (const ElementPtr& elem : doc->getCollections())
        {
            if (!filter || filter(elem))
            {
                readCollection(elem->asA<Collection>(), rootPrim, stage, mapper);
            }
        }

        // Read looks
        for (const LookPtr& elem : doc->getLooks())
        {
            if (!filter || filter(elem))
            {
                readLook(elem, rootPrim, stage, mapper);
            }
        }

        // Read look groups
        for (const LookGroupPtr& elem : doc->getLookGroups())
        {
            if (!filter || filter(elem))
            {
                readLookGroup(elem, rootPrim, stage, mapper);
            }
        }

        // Create additional connections
        makeCollectionIncludeConnections(doc->getCollections(), rootPrim, mapper);
        makeLookInheritConnections(doc->getLooks(), rootPrim, mapper);
    }

    void validateNodesHaveNodedefs(DocumentPtr doc)
    {
        for (const ElementPtr& elem : doc->getChildren())
        {
            if (elem->isA<Node>())
            {
                NodePtr node = elem->asA<Node>();
                const RtToken nodedefName = resolveNodeDefName(node);
                if (nodedefName == EMPTY_TOKEN)
                {
                    throw ExceptionRuntimeError("No matching nodedef was found for node '" + node->getName() + "'");
                }
            }
        }
    }

#ifdef MATERIALX_BUILD_USD
    void readUsdStage(const pxr::UsdStageRefPtr& inStage, PvtStage* stage, const RtReadOptions* readOptions)
    {
        // TODO: is is needed for USD?
        stage->addSourceUri(RtToken(inStage->GetRootLayer()->GetIdentifier()));

        // TODO: read USD root layer metadata?
        // TODO: filters

        // Get all the materials in the USD stage
        std::vector<pxr::UsdPrim> materials;
        pxr::UsdPrimRange range = inStage->Traverse();
        std::copy_if(range.begin(), range.end(), std::back_inserter(materials),
                 [](pxr::UsdPrim const &prim) {
                     return prim.IsA<pxr::UsdShadeNodeGraph>();
                 });

        for(const auto& prim : materials)
        {
            readUsdMaterial(prim, stage->getRootPrim(), stage);
        }
    }
#endif

    void readDocument(const DocumentPtr& doc, PvtStage* stage, const RtReadOptions* readOptions)
    {
        // Set the source location 
        const std::string& uri = doc->getSourceUri();
        stage->addSourceUri(RtToken(uri));

        readMetadata(doc, stage->getRootPrim(), stageMetadata);

        RtReadOptions::ReadFilter filter = readOptions ? readOptions->readFilter : nullptr;

        // First, load and register all nodedefs.
        // Having these available is needed when node instances are loaded later.
        for (const NodeDefPtr& nodedef : doc->getNodeDefs())
        {
            if (!filter || filter(nodedef))
            {
                if (!RtApi::get().hasMasterPrim(RtToken(nodedef->getName())))
                {
                    PvtPrim* prim = readNodeDef(nodedef, stage);
                    RtNodeDef(prim->hnd()).registerMasterPrim();
                }
            }
        }

        validateNodesHaveNodedefs(doc);

        // Keep track of renamed nodes:
        PvtRenamingMapper mapper;

        // Load all other elements.
        for (const ElementPtr& elem : doc->getChildren())
        {
            if (!filter || filter(elem))
            {
                if (elem->isA<Node>())
                {
                    readNode(elem->asA<Node>(), stage->getRootPrim(), stage, mapper);
                }
                else if (elem->isA<NodeGraph>())
                {
                    // Always skip if the nodegraph implements a nodedef
                    PvtPath path(PvtPath::ROOT_NAME.str() + elem->getName());
                    if (stage->getPrimAtPath(path) && elem->asA<NodeGraph>()->getNodeDef())
                    {
                        continue;
                    }
                    readNodeGraph(elem->asA<NodeGraph>(), stage->getRootPrim(), stage, mapper);
                }
                else
                {
                    const RtToken category(elem->getCategory());
                    if (category != RtLook::typeName() &&
                        category != RtLookGroup::typeName() &&
                        category != RtMaterialAssign::typeName() &&
                        category != RtCollection::typeName() &&
                        category != RtNodeDef::typeName()) {
                        readGenericPrim(elem, stage->getRootPrim(), stage, mapper);
                    }
                }
            }
        }

        // Create connections between all root level nodes.
        createNodeConnections(doc->getNodes(), stage->getRootPrim(), mapper);

        // Read look information
        if (!readOptions || readOptions->readLookInformation)
        {
            readLookInformation(doc, stage, readOptions, mapper);
        }
    }

    void writeNodeDef(const PvtPrim* src, DocumentPtr dest)
    {
        RtNodeDef nodedef(src->hnd());

        NodeDefPtr destNodeDef = dest->addNodeDef(nodedef.getName(), EMPTY_STRING, nodedef.getNode());
        writeMetadata(src, destNodeDef, nodedefMetadata);

        for (const PvtDataHandle attrH : src->getAllAttributes())
        {
            const PvtAttribute* attr = attrH->asA<PvtAttribute>();

            ValueElementPtr destPort;
            if (attr->isA<PvtInput>())
            {
                const PvtInput* input = attr->asA<PvtInput>();
                if (input->isUniform())
                {
                    destPort = destNodeDef->addParameter(attr->getName(), attr->getType().str());
                }
                else
                {
                    destPort = destNodeDef->addInput(attr->getName(), attr->getType().str());
                }
            }
            else
            {
                destPort = destNodeDef->addOutput(attr->getName(), attr->getType().str());
            }

            destPort->setValueString(attr->getValueString());

            if (attr->getColorSpace())
            {
                destPort->setColorSpace(attr->getColorSpace().str());
            }
            if (attr->getUnit())
            {
                destPort->setUnit(attr->getUnit().str());
            }
            if (attr->getUnitType())
            {
                destPort->setUnitType(attr->getUnitType().str());
            }

            writeMetadata(attr, destPort, attrMetadata);
        }
    }

    NodePtr writeNode(const PvtPrim* src, GraphElementPtr dest)
    {
        RtNode node(src->hnd());
        RtNodeDef nodedef(node.getNodeDef());
        if (!nodedef)
        {
            throw ExceptionRuntimeError("Prim '" + src->getName().str() + "' is not a node with a valid nodedef");
        }

        // Count output and get output type
        size_t numOutputs = 0;
        string outputType;
        RtObjTypePredicate<RtOutput> outputFilter;
        for (RtAttribute attr : nodedef.getPrim().getAttributes(outputFilter))
        {
            numOutputs++;
            outputType = attr.getType();
        }

        NodePtr destNode = dest->addNode(nodedef.getNode(), node.getName(), numOutputs > 1 ? "multioutput" : outputType);

        for (RtAttribute attrDef : nodedef.getPrim().getAttributes())
        {
            RtAttribute attr = node.getPrim().getAttribute(attrDef.getName());
            RtInput input = attr.asA<RtInput>();
            if (input)
            {
                // Write input if it's connected or different from default value.
                if (input.isConnected() || !RtValue::compare(input.getType(), input.getValue(), attrDef.getValue()))
                {
                    ValueElementPtr valueElem;
                    if (input.isUniform())
                    {
                        valueElem = destNode->addParameter(input.getName(), input.getType());
                        if (input.isConnected())
                        {
                            RtOutput source = input.getConnection();
                            if (source.isSocket())
                            {
                                // This is a connection to the internal socket of a graph
                                valueElem->setInterfaceName(source.getName());
                            }
                        }
                        else
                        {
                            valueElem->setValueString(input.getValueString());
                        }
                    }
                    else
                    {
                        valueElem = destNode->addInput(input.getName(), input.getType());
                        if (input.isConnected())
                        {
                            RtOutput source = input.getConnection();
                            if (source.isSocket())
                            {
                                // This is a connection to the internal socket of a graph
                                valueElem->setInterfaceName(source.getName());
                            }
                            else
                            {
                                RtPrim sourceNode = source.getParent();
                                InputPtr inputElem = valueElem->asA<Input>();
                                if (sourceNode.hasApi<RtNodeGraph>())
                                {
                                    inputElem->setNodeGraphName(
                                        sourceNode.getName());
                                    inputElem->setOutputString(
                                        source.getName());
                                }
                                else
                                {
                                    inputElem->setNodeName(sourceNode.getName());
                                    RtNode rtSourceNode(sourceNode);
                                    int numSourceNodeOutputs = 0;
                                    auto outputIter = rtSourceNode.getOutputs();
                                    while (!outputIter.isDone())
                                    {
                                        numSourceNodeOutputs++;
                                        ++outputIter;
                                    }
                                    if (numSourceNodeOutputs > 1)
                                    {
                                        inputElem->setOutputString(source.getName());
                                    }
                                }
                            }
                        }
                        else
                        {
                            valueElem->setValueString(input.getValueString());
                        }
                    }

                    const RtToken colorspace = input.getColorSpace();
                    if (colorspace != EMPTY_TOKEN)
                    {
                        valueElem->setColorSpace(colorspace.str());
                    }
                    const RtToken unit = input.getUnit();
                    if (unit != EMPTY_TOKEN)
                    {
                        valueElem->setUnit(unit.str());
                    }
                    const RtToken unitType = input.getUnitType();
                    if (unitType != EMPTY_TOKEN)
                    {
                        valueElem->setUnitType(unitType.str());
                    }
                }
            }
            else if(numOutputs > 1)
            {
                destNode->addOutput(attr.getName(), attr.getType());
            }
        }

        writeMetadata(src, destNode, nodeMetadata);

        return destNode;
    }

    void writeMaterialElement(NodePtr mxNode, DocumentPtr doc, const RtWriteOptions* writeOptions)
    {
        string uniqueName = doc->createValidChildName(mxNode->getName() + "_Material");
        string materialName = mxNode->getName();
        mxNode->setName(uniqueName);

        InputPtr materialNodeSurfaceShaderInput = mxNode->getInput(RtType::SURFACESHADER);
        NodePtr surfaceShader = materialNodeSurfaceShaderInput->getConnectedNode();
        if (surfaceShader)
        {
            MaterialPtr material = doc->addMaterial(materialName);
            ShaderRefPtr shaderRef =
                material->addShaderRef(surfaceShader->getName(), surfaceShader->getCategory());

            for (InputPtr input : surfaceShader->getActiveInputs())
            {
                BindInputPtr bindInput = shaderRef->addBindInput(input->getName(), input->getType());
                if (input->hasNodeGraphName() && input->hasOutputString() && doc->getNodeGraph(input->getNodeGraphName()))
                {
                    bindInput->setNodeGraphString(input->getNodeGraphName());
                    bindInput->setOutputString(input->getOutputString());
                }
                else if(input->hasNodeName())
                {
                    const auto outputName = std::string(OUTPUT_ELEMENT_PREFIX.c_str()) +
                                            input->getNodeName() + "_out";
                    if (!doc->getOutput(outputName)) {
                        auto output = doc->addOutput(outputName, input->getType());
                        output->setNodeName(input->getNodeName());
                        auto srcNode = input->getConnectedNode();
                        if (srcNode->getOutputs().size() > 1)
                        {
                            output->setOutputString(input->getOutputString());
                        }
                    }
                    bindInput->setOutputString(outputName);
                }
                else
                {
                    bindInput->setValueString(input->getValueString());
                }
            }
            for (ParameterPtr param : surfaceShader->getActiveParameters())
            {
                BindParamPtr bindParam = shaderRef->addBindParam(param->getName(), param->getType());
                bindParam->setValueString(param->getValueString());
            }

            // Should we create a look for the material element?
            if (writeOptions->materialWriteOp & RtWriteOptions::MaterialWriteOp::CREATE_LOOKS)
            {
                LookPtr look = doc->addLook();
                MaterialAssignPtr materialAssign = look->addMaterialAssign();
                materialAssign->setMaterial(materialName);
                CollectionPtr collection = doc->addCollection();
                collection->setIncludeGeom("/*");
                materialAssign->setCollection(collection);
            }
        }

        doc->removeChild(uniqueName);
    }

    void writeNodeGraph(const PvtPrim * src, DocumentPtr dest)
    {
        NodeGraphPtr destNodeGraph = dest->addNodeGraph(src->getName());
        writeMetadata(src, destNodeGraph, nodegraphMetadata);

        RtNodeGraph nodegraph(src->hnd());

        // Write nodes.
        for (RtPrim node : nodegraph.getNodes())
        {
            writeNode(PvtObject::ptr<PvtPrim>(node), destNodeGraph);
        }

        // Write outputs.
        RtObjTypePredicate<RtOutput> outputsFilter;
        for (RtAttribute attr : src->getAttributes(outputsFilter))
        {
            RtInput nodegraphOutput = nodegraph.getOutputSocket(attr.getName());
            OutputPtr output = destNodeGraph->addOutput(nodegraphOutput.getName(), nodegraphOutput.getType());
            if (nodegraphOutput.isConnected())
            {
                RtOutput source = nodegraphOutput.getConnection();
                if (source.isSocket())
                {
                    output->setInterfaceName(source.getName());
                }
                else
                {
                    RtPrim sourceNode = source.getParent();
                    output->setNodeName(sourceNode.getName());
                    RtNode rtSourceNode(sourceNode);
                    int numSourceNodeOutputs = 0;
                    auto outputIter = rtSourceNode.getOutputs();
                    while (!outputIter.isDone())
                    {
                        numSourceNodeOutputs++;
                        ++outputIter;
                    }
                    if (numSourceNodeOutputs > 1)
                    {
                        output->setOutputString(source.getName());
                    }
                }
            }
        }
    }

    void writeCollections(PvtStage* stage, Document& dest, RtWriteOptions::WriteFilter filter)
    {
        for (RtPrim child : stage->getRootPrim()->getChildren(filter))
        {
            const PvtPrim* prim = PvtObject::ptr<PvtPrim>(child);
            const RtToken typeName = child.getTypeInfo()->getShortTypeName();
            if (typeName == RtCollection::typeName())
            {
                RtCollection rtCollection(prim->hnd());
                const string name(prim->getName());

                if (dest.getCollection(name))
                {
                    continue;
                }
                CollectionPtr collection = dest.addCollection(name);
                collection->setExcludeGeom(rtCollection.getExcludeGeom().getValueString());
                collection->setIncludeGeom(rtCollection.getIncludeGeom().getValueString());

                RtRelationship rtIncludeCollection = rtCollection.getIncludeCollection();
                string includeList = rtIncludeCollection.getTargetsAsString();                
                collection->setIncludeCollectionString(includeList);
            }
        }
    }

    void writeLooks(PvtStage* stage, Document& dest, RtWriteOptions::WriteFilter filter)
    {
        for (RtPrim child : stage->getRootPrim()->getChildren(filter))
        {
            const PvtPrim* prim = PvtObject::ptr<PvtPrim>(child);
            const RtToken typeName = child.getTypeInfo()->getShortTypeName();
            if (typeName == RtLook::typeName())
            {
                RtLook rtLook(prim->hnd());
                const string name(prim->getName());

                if (dest.getCollection(name))
                {
                    continue;
                }
                LookPtr look = dest.addLook(name);

                // Add inherit
                if (!rtLook.getInherit().getTargetsAsString().empty())
                {
                    look->setInheritString(rtLook.getInherit().getTargetsAsString());
                }

                // Add in material assignments
                for (const RtObject& obj : rtLook.getMaterialAssigns().getTargets())
                {
                    PvtPrim* pprim = PvtObject::ptr<PvtPrim>(obj);
                    RtMaterialAssign rtMatAssign(pprim->hnd());

                    const string& assignName = rtMatAssign.getName();
                    if (look->getMaterialAssign(assignName))
                    {
                        continue;
                    }

                    MaterialAssignPtr massign = look->addMaterialAssign(assignName);
                    if (massign)
                    {
                        massign->setExclusive(rtMatAssign.getExclusive().getValue().asBool());
                        auto iter = rtMatAssign.getCollection().getTargets();
                        if (!iter.isDone()) {
                            massign->setCollectionString((*iter).getName());
                        }

                        iter = rtMatAssign.getMaterial().getTargets();
                        if (!iter.isDone()) {
                            massign->setMaterial((*iter).getName());
                        }
                        massign->setGeom(rtMatAssign.getGeom().getValueString());
                    }
                }
            }
        }
    }

    void writeLookGroups(PvtStage* stage, Document& dest, RtWriteOptions::WriteFilter filter)
    {
        for (RtPrim child : stage->getRootPrim()->getChildren(filter))
        {
            const PvtPrim* prim = PvtObject::ptr<PvtPrim>(child);
            const RtToken typeName = child.getTypeInfo()->getShortTypeName();
            if (typeName == RtLookGroup::typeName())
            {
                RtLookGroup rtLookGroup(prim->hnd());
                const string name(rtLookGroup.getName());

                if (dest.getLookGroup(name))
                {
                    continue;
                }
                LookGroupPtr lookGroup = dest.addLookGroup(name);
                string lookList = rtLookGroup.getLooks().getTargetsAsString();
                lookGroup->setLooks(lookList);
                lookGroup->setActiveLook(rtLookGroup.getActiveLook().getValueString());
            }
        }
    }

    void writeGenericPrim(const PvtPrim* src, ElementPtr dest)
    {
        RtGeneric generic(src->hnd());

        ElementPtr elem = dest->addChildOfCategory(generic.getKind(), generic.getName());
        writeMetadata(src, elem, genericMetadata);

        for (auto child : src->getChildren())
        {
            writeGenericPrim(PvtObject::ptr<PvtPrim>(child), elem);
        }
    }

    void writeSourceUris(const PvtStage* stage, DocumentPtr doc)
    {
        for (const RtStagePtr& refPtr : stage->getAllReferences())
        {
            const PvtStage* ref = PvtStage::ptr(refPtr);
            if (ref->getAllReferences().size() > 0)
            {
                writeSourceUris(ref, doc);
            }
            const RtTokenVec& uris = ref->getSourceUri();
            if (!uris.empty())
            {
                for (const RtToken& uri : uris)
                {
                    prependXInclude(doc, uri.str());
                }
            }
        }
    }

    void writeDocument(DocumentPtr& doc, PvtStage* stage, const RtWriteOptions* writeOptions)
    {
        writeMetadata(stage->getRootPrim(), doc, RtTokenSet());

        // Write out any dependent includes
        if (writeOptions && writeOptions->writeIncludes)
        {
            writeSourceUris(stage, doc);
        }

        RtWriteOptions::WriteFilter filter = writeOptions ? writeOptions->writeFilter : nullptr;
        std::vector<NodePtr> materialElements;
        for (RtPrim child : stage->getRootPrim()->getChildren(filter))
        {
            const PvtPrim* prim = PvtObject::ptr<PvtPrim>(child);
            const RtToken typeName = child.getTypeInfo()->getShortTypeName();
            if (typeName == RtNodeDef::typeName())
            {
                writeNodeDef(prim, doc);
            }
            else if (typeName == RtNode::typeName())
            {
                NodePtr mxNode = writeNode(prim, doc);
                RtNode node(prim->hnd());
                const RtOutput& output = node.getOutput(DEFAULT_OUTPUT);
                if (output && output.getType() == MATERIAL_TYPE_STRING && writeOptions &&
                    writeOptions->materialWriteOp & RtWriteOptions::MaterialWriteOp::WRITE_MATERIALS_AS_ELEMENTS)
                {
                    materialElements.push_back(mxNode);
                }
            }
            else if (typeName == RtNodeGraph::typeName())
            {
                writeNodeGraph(prim, doc);
            }
            else if (typeName != RtLook::typeName() &&
                     typeName != RtLookGroup::typeName() &&
                     typeName != RtMaterialAssign::typeName() &&
                     typeName != RtCollection::typeName())
            {
                writeGenericPrim(prim, doc->asA<Element>());
            }
        }

        // Write the existing look information
        if (!writeOptions || 
            (writeOptions->materialWriteOp & RtWriteOptions::MaterialWriteOp::WRITE_LOOKS) ||
            (writeOptions->materialWriteOp & RtWriteOptions::MaterialWriteOp::CREATE_LOOKS))
        {
            writeCollections(stage, *doc, filter);
            writeLooks(stage, *doc, filter);
            writeLookGroups(stage, *doc, filter);
        }

        for (auto & mxNode: materialElements) {
            writeMaterialElement(mxNode, doc, writeOptions);
        }
    }

    void readUnitDefinitions(DocumentPtr doc)
    {
        RtApi& api = RtApi::get();
        UnitConverterRegistryPtr unitDefinitions = api.getUnitDefinitions();
        for (UnitTypeDefPtr unitTypeDef : doc->getUnitTypeDefs())
        {
            LinearUnitConverterPtr unitConvert = LinearUnitConverter::create(unitTypeDef);
            unitDefinitions->addUnitConverter(unitTypeDef, unitConvert);
        }
    }

    void writeUnitDefinitions(DocumentPtr doc)
    {
        RtApi& api = RtApi::get();
        UnitConverterRegistryPtr unitDefinitions = api.getUnitDefinitions();
        if (unitDefinitions)
        {
            unitDefinitions->write(doc);
        }
    }

    std::vector<RtNode> getMaterialNodes(const RtStagePtr& mxStage)
    {
        std::vector<RtNode> result;
        RtSchemaPredicate<RtNode> nodeFilter;
        for (auto it = mxStage->traverse(nodeFilter); !it.isDone(); ++it)
        {
            RtNode node((*it));
            RtNodeDef nodedef(node.getNodeDef());
            if( nodedef.getName() == "ND_surfacematerial" || 
                nodedef.getName() == "ND_volumematerial")
            {
                result.push_back(node);
            }
        }
        return result;
    }

    void setUsdShader(const RtNode& mxShader, const RtInput& input, pxr::UsdShadeShader& usdShader)
    {
        ValueElementPtr valueElem;

        RtValue val(input.getValue());
        if(input.getType() == RtToken("float"))
        {
            usdShader.CreateInput(pxr::TfToken(input.getName().c_str()), 
                pxr::SdfValueTypeNames->Float).Set(val.asFloat());
        }
        else if(input.getType() == RtToken("color3"))
        {
            pxr::VtValue vtVal(pxr::GfVec3f(val.asColor3()[0], val.asColor3()[1], val.asColor3()[2]));
            usdShader.CreateInput(pxr::TfToken(input.getName().c_str()), 
                pxr::SdfValueTypeNames->Color3f).Set(vtVal);
        }
        else if(input.getType() == RtToken("filename"))
        {
            pxr::UsdShadeInput usdFileInput = usdShader.CreateInput(pxr::TfToken(input.getName().c_str()), pxr::SdfValueTypeNames->Asset);
            usdFileInput.Set(pxr::VtValue(pxr::SdfAssetPath(val.asString())));
        }
    }

    void createUsdShaderFromShader(const RtInput& mxInput, const pxr::SdfPath& usdMaterialPath, pxr::UsdShadeShader& inUsdShader)
    {
        RtOutput mxShaderOuput = mxInput.getConnection();
        RtPrim parent = mxShaderOuput.getParent();
        RtNode mxShader(parent);

        pxr::UsdStageRefPtr usdStage = inUsdShader.GetPrim().GetStage();
        pxr::SdfPath usdShaderPath = usdMaterialPath.AppendPath(pxr::SdfPath(mxShader.getName()));
        pxr::UsdShadeShader usdShader = pxr::UsdShadeShader::Get(usdStage, usdShaderPath);
        RtNodeDef nodedef(mxShader.getNodeDef());
        if(!usdShader)
        {
            usdShader = pxr::UsdShadeShader::Define(
                usdStage,
                usdShaderPath
            );
            usdShader.CreateIdAttr(pxr::VtValue(pxr::TfToken(nodedef.getName())));
        }
        else
        {
            pxr::TfToken id;
            usdShader.GetShaderId(&id);
            if(id.GetString() != nodedef.getName().str())
            {
                return;
            }
        }

        // connect shader output to input
        pxr::SdfValueTypeName usdType;
        if(mxInput.getType() == "color3")
        {
            usdType = pxr::SdfValueTypeNames->Color3f;
        }
        else if(mxInput.getType() == "vector3")
        {
            usdType = pxr::SdfValueTypeNames->Vector3f;
        }
        else if(mxInput.getType() == "float")
        {
            usdType = pxr::SdfValueTypeNames->Float;
        }

        if(usdType)
        {
            inUsdShader.CreateInput(
                pxr::TfToken(mxInput.getName()), usdType).ConnectToSource(
                usdShader, pxr::TfToken(mxShaderOuput.getName()));
        }

        for (RtAttribute attrDef : nodedef.getPrim().getAttributes())
        {
            RtAttribute attr = mxShader.getPrim().getAttribute(attrDef.getName());
            RtInput input = attr.asA<RtInput>();         
            if (!input)
            {
                continue;
            }


            // Write input if it's connected or different from default value.
            if (!RtValue::compare(input.getType(), input.getValue(), attrDef.getValue()))
            {
                setUsdShader(mxShader, input, usdShader);
            }
            else if(input.isConnected())
            {
                createUsdShaderFromShader(input, usdMaterialPath, usdShader);
            }            
        }
    }

    void createUsdShaderFromMaterial(const RtInput& mxInput, pxr::UsdShadeMaterial& usdMaterial)
    {
        RtOutput mxShaderOuput = mxInput.getConnection();
        RtPrim parent = mxShaderOuput.getParent();
        RtNode mxShader(parent);
        RtNodeDef nodedef(mxShader.getNodeDef());

        pxr::UsdStageRefPtr usdStage = usdMaterial.GetPrim().GetStage();
        pxr::SdfPath usdShaderPath = usdMaterial.GetPath().AppendPath(pxr::SdfPath(mxShader.getName()));
        pxr::UsdShadeShader usdShader = pxr::UsdShadeShader::Get(usdStage, usdShaderPath);
        if(!usdShader)
        {
            usdShader = pxr::UsdShadeShader::Define(usdStage, usdShaderPath);
            usdShader.CreateIdAttr(pxr::VtValue(pxr::TfToken(nodedef.getName())));
        }
        else
        {
            pxr::TfToken id;
            usdShader.GetShaderId(&id);
            if(id.GetString() != nodedef.getName().str())
            {
                return;
            }
        }

        // connect the Usd shader to the Usd material surface or displacement
        if(mxInput.getName() == "surfaceshader")
        {
            usdMaterial.CreateSurfaceOutput().ConnectToSource(usdShader, pxr::TfToken(mxShaderOuput.getName()));
        }
        else if(mxInput.getName() == "displacementshader")
        {
            usdMaterial.CreateDisplacementOutput().ConnectToSource(usdShader, pxr::TfToken(mxShaderOuput.getName()));
        }

        for (RtAttribute attrDef : nodedef.getPrim().getAttributes())
        {            
            RtAttribute attr = mxShader.getPrim().getAttribute(attrDef.getName());
            RtInput input = attr.asA<RtInput>();         
            if (!input)
            {
                continue;
            }

            if (!RtValue::compare(input.getType(), input.getValue(), attrDef.getValue()))
            {
                setUsdShader(mxShader, input, usdShader);
            }
            else if(input.isConnected())
            {
                createUsdShaderFromShader(input, usdMaterial.GetPath(), usdShader);
            }
        }
    }

    void createUsdMaterialNetwork(const RtNode& mxMaterial, pxr::UsdPrim& materialxPrim)
    {
        pxr::UsdShadeMaterial usdMaterial = pxr::UsdShadeMaterial::Define(
            materialxPrim.GetStage(),
            materialxPrim.GetPath().AppendPath(pxr::SdfPath(mxMaterial.getName()))
        );

        for(auto attr : mxMaterial.getInputs())
        {
            RtInput mxInput = attr.asA<RtInput>();
            if(mxInput.isConnected())
            {
                createUsdShaderFromMaterial(mxInput, usdMaterial);
            }
        }

    }

    void rtStageToUsdStage(const RtStagePtr& mxStage, pxr::UsdStageRefPtr& usdStage)
    {
        // Every MaterialX nodes will be under a 'MaterialX' UsdPrim
        pxr::UsdPrim materialxPrim = usdStage->DefinePrim(
            pxr::SdfPath("/MaterialX"), pxr::TfToken("Scope"));


        std::vector<RtNode> mxMaterials = getMaterialNodes(mxStage);
        for(const auto& node : mxMaterials)
        {
            createUsdMaterialNetwork(node, materialxPrim);
        }    
    }

    void writeToUsdFile(const FilePath& documentPath, const RtStagePtr& mxStage)
    {
        pxr::UsdStageRefPtr usdStage = pxr::UsdStage::CreateNew(documentPath.asString());
        rtStageToUsdStage(mxStage, usdStage);

        usdStage->Save();
    }
} // end anonymous namespace

RtReadOptions::RtReadOptions() :
    readFilter(nullptr),
    readLookInformation(false),
    desiredMajorVersion(MATERIALX_MAJOR_VERSION),
    desiredMinorVersion(MATERIALX_MINOR_VERSION + 1)
{
}

RtWriteOptions::RtWriteOptions() :
    writeIncludes(true),
    writeFilter(nullptr),
    materialWriteOp(NONE),
    desiredMajorVersion(MATERIALX_MAJOR_VERSION),
    desiredMinorVersion(MATERIALX_MINOR_VERSION + 1)
{
}

void RtFileIo::read(const FilePath& documentPath, const FileSearchPath& searchPaths, const RtReadOptions* readOptions)
{
    if(documentPath.getExtension() == "mtlx")
    {
        try
        {
            DocumentPtr document = createDocument();
            XmlReadOptions xmlReadOptions;
            xmlReadOptions.skipConflictingElements = true;
            if (readOptions)
            {
                xmlReadOptions.skipConflictingElements = true;
                xmlReadOptions.desiredMajorVersion = readOptions->desiredMajorVersion;
                xmlReadOptions.desiredMinorVersion = readOptions->desiredMinorVersion;
            }
            readFromXmlFile(document, documentPath, searchPaths, &xmlReadOptions);

            string errorMessage;
            DocumentPtr validationDocument = createDocument();
            writeUnitDefinitions(validationDocument);
            CopyOptions cops;
            cops.skipConflictingElements = true;
            validationDocument->copyContentFrom(document, &cops);
            if (validationDocument->validate(&errorMessage))
            {
                PvtStage* stage = PvtStage::ptr(_stage);
                readDocument(document, stage, readOptions);
            }
            else
            {
                throw ExceptionRuntimeError("Failed validation: " + errorMessage);
            }
        }
        catch (Exception& e)
        {
            throw ExceptionRuntimeError("Could not read file: " + documentPath.asString() + ". Error: " + e.what());
        }
    }
#ifdef MATERIALX_BUILD_USD
    else if(documentPath.getExtension() == "usd" || documentPath.getExtension() == "usda")
    {
        try
        {
            FilePath resolvedFilePath = searchPaths.find(documentPath);
            pxr::UsdStageRefPtr usdStage = pxr::UsdStage::Open(resolvedFilePath.asString());
            string errorMessage;
            if(usdStage)
            {
                PvtStage* stage = PvtStage::ptr(_stage);
                readUsdStage(usdStage, stage, readOptions);
            }
            else
            {
                throw ExceptionRuntimeError("USD Note implemented: " + errorMessage);
            }

        }
        catch (Exception& e)
        {
            throw ExceptionRuntimeError("Could not read file: " + documentPath.asString() + ". Error: " + e.what());
        }
    }
#endif
}

void RtFileIo::read(std::istream& stream, const RtReadOptions* readOptions)
{
    try
    {
        DocumentPtr document = createDocument();
        XmlReadOptions xmlReadOptions;
        xmlReadOptions.skipConflictingElements = true;
        if (readOptions)
        {
            xmlReadOptions.skipConflictingElements = true;
            xmlReadOptions.desiredMajorVersion = readOptions->desiredMajorVersion;
            xmlReadOptions.desiredMajorVersion = readOptions->desiredMinorVersion;
        }
        readFromXmlStream(document, stream, &xmlReadOptions);

        string errorMessage; 
        DocumentPtr validationDocument = createDocument();
        writeUnitDefinitions(validationDocument);
        CopyOptions cops;
        cops.skipConflictingElements = true;
        validationDocument->copyContentFrom(document, &cops);
        if (validationDocument->validate(&errorMessage))
        {
            PvtStage* stage = PvtStage::ptr(_stage);
            readDocument(document, stage, readOptions);
        }
        else
        {
            throw ExceptionRuntimeError("Failed validation: " + errorMessage);
        }

    }
    catch (Exception& e)
    {
        throw ExceptionRuntimeError(string("Could not read from stream. Error: ") + e.what());
    }
}

void RtFileIo::readLibraries(const StringVec& libraryPaths, const FileSearchPath& searchPaths)
{
    PvtStage* stage = PvtStage::ptr(_stage);

    // Load all content into a document.
    DocumentPtr doc = createDocument();
    MaterialX::loadLibraries(libraryPaths, searchPaths, doc);

    StringSet uris = doc->getReferencedSourceUris();
    for (const string& uri : uris)
    {
        stage->addSourceUri(RtToken(uri));
    }

    // Update any units found
    readUnitDefinitions(doc);

    // First, load all nodedefs. Having these available is needed
    // when node instances are loaded later.
    for (const NodeDefPtr& nodedef : doc->getNodeDefs())
    {
        if (!RtApi::get().hasMasterPrim(RtToken(nodedef->getName())))
        {
            PvtPrim* prim = readNodeDef(nodedef, stage);
            RtNodeDef(prim->hnd()).registerMasterPrim();
        }
    }

    validateNodesHaveNodedefs(doc);

    // We were already renaming on conflict here. Keep track of the new names.
    PvtRenamingMapper mapper;

    // Second, load all other elements.
    for (const ElementPtr& elem : doc->getChildren())
    {
        PvtPath path(stage->getPath());
        path.push(RtToken(elem->getName()));

        if (elem->isA<NodeDef>() || stage->getPrimAtPath(path))
        {
            continue;
        }

        if (elem->isA<Node>())
        {
            readNode(elem->asA<Node>(), stage->getRootPrim(), stage, mapper);
        }
        else if (elem->isA<NodeGraph>())
        {
            readNodeGraph(elem->asA<NodeGraph>(), stage->getRootPrim(), stage, mapper);
        }
        else
        {
            readGenericPrim(elem, stage->getRootPrim(), stage, mapper);
        }
    }
}

void RtFileIo::write(const FilePath& documentPath, const RtWriteOptions* options)
{
    PvtStage* stage = PvtStage::ptr(_stage);

    if(documentPath.getExtension() == "mtlx")
    {
        DocumentPtr document = createDocument();
        writeDocument(document, stage, options);

        XmlWriteOptions xmlWriteOptions;
        if (options)
        {
            xmlWriteOptions.writeXIncludeEnable = options->writeIncludes;
            document->setVersionString(makeVersionString(options->desiredMajorVersion, options->desiredMinorVersion));
        }
        else
        {
            document->setVersionString(makeVersionString(MATERIALX_MAJOR_VERSION, MATERIALX_MINOR_VERSION + 1));
        }
        writeToXmlFile(document, documentPath, &xmlWriteOptions);
    }
#ifdef MATERIALX_BUILD_USD
    else if(documentPath.getExtension() == "usd" || documentPath.getExtension() == "usda")
    {
        writeToUsdFile(documentPath, _stage);
    }
#endif    
}

void RtFileIo::write(std::ostream& stream, const RtWriteOptions* options)
{
    PvtStage* stage = PvtStage::ptr(_stage);

    DocumentPtr document = createDocument();
    writeDocument(document, stage, options);

    XmlWriteOptions xmlWriteOptions;
    if (options)
    {
        xmlWriteOptions.writeXIncludeEnable = options->writeIncludes;
        document->setVersionString(makeVersionString(options->desiredMajorVersion, options->desiredMinorVersion));
    }
    else
    {
        document->setVersionString(makeVersionString(MATERIALX_MAJOR_VERSION, MATERIALX_MINOR_VERSION + 1));
    }
    writeToXmlStream(document, stream, &xmlWriteOptions);
}

}
