//
// TM & (c) 2017 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved.  See LICENSE.txt for license.
//

#include <MaterialXRender/TinyGLTFLoader.h>
#include <MaterialXCore/Util.h>

// Want implementation but not image capabilities
#define TINYGLTF_IMPLEMENTATION 1
#define TINYGLTF_NO_STB_IMAGE 1
#define TINYGLTF_NO_STB_IMAGE_WRITE 1
#define TINYGLTF_NO_EXTERNAL_IMAGE 1

#include <MaterialXRender/External/TinyGLTFLoader/tiny_gltf.h>

#include <iostream>

namespace MaterialX
{

namespace {

const float MAX_FLOAT = std::numeric_limits<float>::max();
const size_t FACE_VERTEX_COUNT = 3;

class VertexVector : public VectorN<VertexVector, float, 8>
{
  public:
    using VectorN<VertexVector, float, 8>::VectorN;
    VertexVector(const Vector3& p, const Vector3& n, const Vector2& t) : VectorN(Uninit{})
    {
        _arr = {p[0], p[1], p[2], n[0], n[1], n[2], t[0], t[1]};
    }
};

using VertexIndexMap = std::unordered_map<VertexVector, uint32_t, VertexVector::Hash>;

} // anonymous namespace

//
// TinyGLTFLoader methods
//
#define GBUFFER_OFFSET(i) ((char *)NULL + (i))

#if 0
static size_t SIZEOF_COMPONENT(int type) {
    switch (type) {
    case TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE:
    case TINYGLTF_COMPONENT_TYPE_BYTE:
        return sizeof(char);
    case TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT:
    case TINYGLTF_COMPONENT_TYPE_SHORT:
        return sizeof(short);
    case TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT:
    case TINYGLTF_COMPONENT_TYPE_INT:
        return sizeof(int);
    case TINYGLTF_COMPONENT_TYPE_FLOAT:
        return sizeof(float);
    case TINYGLTF_COMPONENT_TYPE_DOUBLE:
        return sizeof(double);
    default:
        return 0;
    }
}
#endif

bool TinyGLTFLoader::load(const FilePath& filePath, MeshList& meshList)
{
	tinygltf::Model model;
	tinygltf::TinyGLTF gltf_ctx;
	std::string err;
	std::string warn;
	const std::string input_filename = filePath.asString();

	bool store_original_json_for_extras_and_extensions = false;
	gltf_ctx.SetStoreOriginalJSONForExtrasAndExtensions(
		store_original_json_for_extras_and_extensions);

	const std::string ext = filePath.getExtension();
    const std::string BINARY_EXTENSION = "glb";
    const std::string ASCII_EXTENSION = "gltf";

	bool ret = false;
	if (ext.compare(BINARY_EXTENSION) == 0)
	{
		// Try to read as binary
		ret = gltf_ctx.LoadBinaryFromFile(&model, &err, &warn,
			input_filename.c_str());
	}
	else if (ext.compare(ASCII_EXTENSION) == 0)
    {
        // Try to read as ascii
		ret =
			gltf_ctx.LoadASCIIFromFile(&model, &err, &warn, input_filename.c_str());
	}

	//if (!warn.empty() || !err.empty() || !ret)
	//{
	//	return false;
	//}

    // Load model 
    // For each gltf mesh a new mesh is created
    // A MeshStream == buffer view for an attribute + associated data.
    // A MeshPartition == buffer view for indexing + associated data.
    for (auto gMesh : model.meshes)
    {
        // Create new mesh
        MeshPtr mesh = Mesh::create(gMesh.name);
        meshList.push_back(mesh);
        mesh->setSourceUri(filePath);

        MeshStreamPtr positionStream = MeshStream::create("i_" + MeshStream::POSITION_ATTRIBUTE, MeshStream::POSITION_ATTRIBUTE, 0);
        MeshStreamPtr normalStream = nullptr;
        MeshStreamPtr texcoordStream = MeshStream::create("i_" + MeshStream::TEXCOORD_ATTRIBUTE + "_0", MeshStream::TEXCOORD_ATTRIBUTE, 0);
        MeshStreamPtr tangentStream = nullptr;

        // Scane for primitives on the mesh
        for (auto gPrim : gMesh.primitives)
        {
            // Get index accessor for the prim. Create a partition
            // Only support triangle indexing
            if ((gPrim.indices >= 0) &&
                (gPrim.mode == TINYGLTF_MODE_TRIANGLES))
            {
                const tinygltf::Accessor& gaccessor = model.accessors[gPrim.indices];
                const tinygltf::BufferView& gBufferView = model.bufferViews[gaccessor.bufferView];
                const tinygltf::Buffer& gBuffer = model.buffers[gBufferView.buffer];

                size_t indexCount = gaccessor.count;
                MeshPartitionPtr part = MeshPartition::create();
                size_t faceCount = indexCount / FACE_VERTEX_COUNT;
                part->setFaceCount(faceCount);
                //part->setIdentifier(); - What should this be called ?
                MeshIndexBuffer& indices = part->getIndices();
                indices.resize(indexCount);
                size_t startLocation = gaccessor.byteOffset;
                size_t bytStride = gaccessor.ByteStride(gBufferView);
                gaccessor.componentType;
                for (size_t i = 0; i < indexCount; i++)
                {
                    indices[i] = gBuffer.data[startLocation+(i*bytStride)];
                }
                mesh->addPartition(part);
            }

            // Get attributes. Note that bufferViews contain the content descriptioon
            for (auto& gattrib : gPrim.attributes)
            {
                // Find out the bytstride
                const tinygltf::Accessor& accessor = model.accessors[gattrib.second];
                const tinygltf::BufferView& gBufferView = model.bufferViews[accessor.bufferView];
                const tinygltf::Buffer& gBuffer = model.buffers[gBufferView.buffer];
                //int byteStride = accessor.ByteStride(gBufferView);
                //if (byteStride > -1)
                unsigned int vectorSize = 3;
                if (accessor.type == TINYGLTF_TYPE_VEC2)
                {
                    vectorSize = 2;
                }
                else if (accessor.type == TINYGLTF_TYPE_VEC4)
                {
                    vectorSize = 4;
                }

                if (gattrib.first.compare("POSITION") == 0)
                {
                    // Fill in position stream - TODO
                    MeshFloatBuffer& buffer = positionStream->getData();
                    buffer.resize(accessor.count);
                    memcpy(&buffer[0], &gBuffer.data[0], gBuffer.data.size());
                }
                else if (gattrib.first.compare("NORMAL") == 0)
                {
                    normalStream = MeshStream::create("i_" + MeshStream::NORMAL_ATTRIBUTE, MeshStream::NORMAL_ATTRIBUTE, 0);

                    // Fill in normal stream - TODO
                    MeshFloatBuffer& buffer = normalStream->getData();
                    buffer.resize(accessor.count);
                    memcpy(&buffer[0], &gBuffer.data[0], gBuffer.data.size());
                }
                else if (gattrib.first.compare("TEXCOORD_0") == 0)
                {
                    if (vectorSize == 2)
                    {
                        texcoordStream->setStride(MeshStream::STRIDE_2D);
                    }

                    // Fill in texture coordinate stream - TODO
                    MeshFloatBuffer& buffer = texcoordStream->getData();
                    buffer.resize(accessor.count);
                    memcpy(&buffer[0], &gBuffer.data[0], gBuffer.data.size());
                }
                else if (gattrib.first.compare("TANGENT") == 0)
                {
                    tangentStream = MeshStream::create("i_" + MeshStream::TANGENT_ATTRIBUTE, MeshStream::NORMAL_ATTRIBUTE, 0);

                    // Fill in tangent stream - TODO
                    MeshFloatBuffer& buffer = tangentStream->getData();
                    buffer.resize(accessor.count);
                    memcpy(&buffer[0], &gBuffer.data[0], gBuffer.data.size());
                }
            }
        }

        // General noramsl if none provided
        if (!normalStream)
        {
            normalStream = mesh->generateNormals(positionStream);
        }

        // Generate tangents if none provided
        if (!tangentStream)
        {
            tangentStream = mesh->generateTangents(positionStream, normalStream, texcoordStream);
        }

        // Assign streams to mesh.
        mesh->addStream(positionStream);
        mesh->addStream(normalStream);
        mesh->addStream(texcoordStream);
        if (tangentStream)
        {
            mesh->addStream(tangentStream);
        }

        // Assign properties to mesh.
        mesh->setVertexCount(positionStream->getData().size() / MeshStream::STRIDE_3D);
        Vector3 boxMin = { MAX_FLOAT, MAX_FLOAT, MAX_FLOAT };
        Vector3 boxMax = { -MAX_FLOAT, -MAX_FLOAT, -MAX_FLOAT };
        mesh->setMinimumBounds(boxMin);
        mesh->setMaximumBounds(boxMax);
        Vector3 sphereCenter = (boxMax + boxMin) * 0.5;
        mesh->setSphereCenter(sphereCenter);
        mesh->setSphereRadius((sphereCenter - boxMin).getMagnitude());
    }
	return true;
#if 0    
 
 
    VertexIndexMap vertexIndexMap;
    uint32_t nextVertexIndex = 0;
    bool normalsFound = false;
    for (const tinyobj::shape_t& shape : shapes)
    {
        size_t indexCount = shape.mesh.indices.size();
        if (indexCount == 0)
        {
            continue;
        }
        size_t faceCount = indexCount / FACE_VERTEX_COUNT;

        MeshPartitionPtr part = MeshPartition::create();
        part->setIdentifier(shape.name);
        part->setFaceCount(faceCount);
        mesh->addPartition(part);

        MeshIndexBuffer& indices = part->getIndices();
        indices.resize(indexCount);

        for (size_t i = 0; i < shape.mesh.indices.size(); i++)
        {
            const tinyobj::index_t& indexObj = shape.mesh.indices[i];

            // Read vertex components.
            Vector3 position, normal;
            Vector2 texcoord;
            for (unsigned int k = 0; k < MeshStream::STRIDE_3D; k++)
            {
                position[k] = attrib.vertices[indexObj.vertex_index * MeshStream::STRIDE_3D + k];
                if (indexObj.normal_index >= 0)
                {
                    normal[k] = attrib.normals[indexObj.normal_index * MeshStream::STRIDE_3D + k];
                    normalsFound = true;
                }
                if (indexObj.texcoord_index >= 0 && k < MeshStream::STRIDE_2D)
                {
                    texcoord[k] = attrib.texcoords[indexObj.texcoord_index * MeshStream::STRIDE_2D + k];
                }
            }

            // Check for duplicate vertices.
            VertexVector vec(position, normal, texcoord);
            VertexIndexMap::iterator it = vertexIndexMap.find(vec);
            if (it != vertexIndexMap.end())
            {
                indices[i] = it->second;
                continue;
            }
            vertexIndexMap[vec] = nextVertexIndex;

            // Store vertex components.
            for (unsigned int k = 0; k < MeshStream::STRIDE_3D; k++)
            {
                positionStream->getData().push_back(position[k]);
                normalStream->getData().push_back(normal[k]);
                if (k < MeshStream::STRIDE_2D)
                {
                    texcoordStream->getData().push_back(texcoord[k]);
                }

                // Update bounds.
                boxMin[k] = std::min(position[k], boxMin[k]);
                boxMax[k] = std::max(position[k], boxMax[k]);
            }

            // Store index data.
            indices[i] = nextVertexIndex++;
        }
    }

    // Generate normals if needed.
    if (!normalsFound)
    {
        normalStream = mesh->generateNormals(positionStream);
    }   
#endif
}

} // namespace MaterialX
