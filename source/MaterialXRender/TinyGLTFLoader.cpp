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
#include <algorithm>

namespace MaterialX
{

namespace {

const float MAX_FLOAT = std::numeric_limits<float>::max();
const size_t FACE_VERTEX_COUNT = 3;

uint32_t VALUE_AS_UINT32(int type, const unsigned char* value)
{
    switch (type)
    {
    case TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE:
    case TINYGLTF_COMPONENT_TYPE_BYTE:
        return static_cast<uint32_t>(*value);
    case TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT:
    case TINYGLTF_COMPONENT_TYPE_SHORT:
        return static_cast<uint32_t>(*(short*)value);
    case TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT:
    case TINYGLTF_COMPONENT_TYPE_INT:
        return static_cast<uint32_t>(*(int*)value);
    case TINYGLTF_COMPONENT_TYPE_FLOAT:
        return static_cast<uint32_t>(*(int*)value);
    case TINYGLTF_COMPONENT_TYPE_DOUBLE:
        return static_cast<uint32_t>(*(double*)value);
    default:
        return 0;
    }
}

float VALUE_AS_FLOAT(int type, const unsigned char* value)
{
    switch (type)
    {
    case TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE:
    case TINYGLTF_COMPONENT_TYPE_BYTE:
        return static_cast<float>(*value);
    case TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT:
    case TINYGLTF_COMPONENT_TYPE_SHORT:
        return static_cast<float>(*(short*)value);
    case TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT:
    case TINYGLTF_COMPONENT_TYPE_INT:
        return static_cast<float>(*(int*)value);
    case TINYGLTF_COMPONENT_TYPE_FLOAT:
        return static_cast<float>(*(int*)value);
    case TINYGLTF_COMPONENT_TYPE_DOUBLE:
        return static_cast<float>(*(double*)value);
    default:
        return 0;
    }
}

size_t ComponentTypeByteSize(int type) {
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

} // anonymous namespace

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

	if (!warn.empty() || !err.empty() || !ret)
	{
	    return false;
	}

    Vector3 boxMin = { MAX_FLOAT, MAX_FLOAT, MAX_FLOAT };
    Vector3 boxMax = { -MAX_FLOAT, -MAX_FLOAT, -MAX_FLOAT };

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

        MeshStreamPtr positionStream = nullptr; 
        MeshStreamPtr normalStream = nullptr;
        MeshStreamPtr texcoordStream = nullptr;
        MeshStreamPtr tangentStream = nullptr;

        // Scane for primitives on the mesh
        for (tinygltf::Primitive& gPrim : gMesh.primitives)
        {
            // Get index accessor for the prim. Create a partition
            // Only support triangle indexing
            int accessorIndex = gPrim.indices;
            if ((accessorIndex >= 0) &&
                (gPrim.mode == TINYGLTF_MODE_TRIANGLES))
            {
                const tinygltf::Accessor& gaccessor = model.accessors[accessorIndex];
                const tinygltf::BufferView& gBufferView = model.bufferViews[gaccessor.bufferView];
                const tinygltf::Buffer& gBuffer = model.buffers[gBufferView.buffer];

                size_t indexCount = gaccessor.count;
                MeshPartitionPtr part = MeshPartition::create();
                size_t faceCount = indexCount / FACE_VERTEX_COUNT;
                part->setFaceCount(faceCount);
                part->setIdentifier(gMesh.name); 
                MeshIndexBuffer& indices = part->getIndices();
                indices.resize(indexCount);
                size_t startLocation = gBufferView.byteOffset;
                size_t byteStride = gaccessor.ByteStride(gBufferView);

                bool isTriangleList = false;
                std::string indexingType = "invalid type";
                switch (gPrim.mode) {
                case TINYGLTF_MODE_POINTS:
                    indexingType = "point list";
                    break;
                case TINYGLTF_MODE_LINE:
                    indexingType = "line list";
                    break;
                case TINYGLTF_MODE_LINE_STRIP:
                    indexingType = "line string";
                    break;
                case TINYGLTF_MODE_TRIANGLES:
                    indexingType = "triangle list";
                    isTriangleList = true;
                    break;
                case TINYGLTF_MODE_TRIANGLE_STRIP:
                    indexingType = "triangle strip";
                    break;       
                default:
                    break;
                }
                if (!isTriangleList)
                {
                    std::cout << "Skip unsupported prim type: " << indexingType << " on mesh" <<
                        gMesh.name << std::endl;
                    continue;
                }

                std::cout << "Index start byte offset: " << std::to_string(startLocation) << std::endl;
                std::cout << "-- Index byte stride: " << std::to_string(byteStride) << std::endl;
                //std::cout << "*** Index values: {\n";
                for (size_t i = 0; i < indexCount; i++)
                {
                    size_t offset = startLocation + (i * byteStride);
                    uint32_t bufferIndex = VALUE_AS_UINT32(gaccessor.componentType, &(gBuffer.data[offset]));
                    indices[i] = bufferIndex;
                    //std::cout << "[" + std::to_string(i) + "] = " + std::to_string(bufferIndex) + "\n";
                }
                //std::cout << "}\n";
                mesh->addPartition(part);
            }

            // Get attributes. Note that bufferViews contain the content descriptioon
            for (auto& gattrib : gPrim.attributes)
            {
                // Find out the byteStride
                const tinygltf::Accessor& gAccessor = model.accessors[gattrib.second];
                const tinygltf::BufferView& gBufferView = model.bufferViews[gAccessor.bufferView];
                const tinygltf::Buffer& gBuffer = model.buffers[gBufferView.buffer];
                size_t byteStride = gAccessor.ByteStride(gBufferView);
                size_t floatStride = byteStride / sizeof(float);

                size_t byteOffset = gBufferView.byteOffset + gAccessor.byteOffset;
                size_t startLocation = byteOffset / sizeof(float);

                unsigned int vectorSize = 3;
                if (gAccessor.type == TINYGLTF_TYPE_VEC2)
                {

                    vectorSize = 2;
                }
                else if (gAccessor.type == TINYGLTF_TYPE_VEC4)
                {
                    vectorSize = 4;
                }

                std::cout << "** READ ATTRIB: " << gattrib.first <<  
                    " from buffer: " << std::to_string(gBufferView.buffer) << std::endl;
                std::cout << "-- Buffer start byte offset: " << std::to_string(byteOffset) << std::endl;
                std::cout << "-- Buffer start float offset: " << std::to_string(startLocation) << std::endl;
                std::cout << "-- Byte stride: " << std::to_string(byteStride) << std::endl;
                std::cout << "-- Float stride: " << std::to_string(floatStride) << std::endl;
                std::cout << "-- Vector size: " << std::to_string(vectorSize) << std::endl;

                if (gattrib.first.compare("POSITION") == 0)
                {
                    if (!positionStream)
                    {
                        positionStream = MeshStream::create("i_" + MeshStream::POSITION_ATTRIBUTE, MeshStream::POSITION_ATTRIBUTE, 0);
                    }

                    // Fill in stream 
                    MeshFloatBuffer& buffer = positionStream->getData();

                    // Resize data and get pointer to data as float. 
                    size_t dataCount = gAccessor.count;
                    //buffer.resize(dataCount * vectorSize);

                    const unsigned char* charPointer = &(gBuffer.data[0]);
                    charPointer += byteOffset;
                    float* floatPointer = (float*)charPointer;
                    //std::cout << "{\n";
                    for (size_t i = 0; i < dataCount; i++)
                    {
                        // Copy the vector over
                        //std::cout << "[" + std::to_string(i) + "] = { ";
                        for (size_t v = 0; v < vectorSize; v++)
                        {
                            float bufferData = *(floatPointer + v);
                            buffer.push_back(bufferData);
                            //std::cout << std::to_string(buffer[i]) + " ";

                            // Update bounds.
                            boxMin[v] = std::min(bufferData, boxMin[v]);
                            boxMax[v] = std::max(bufferData, boxMax[v]);
                        }
                        //std::cout << " }" << std::endl;

                        // Jump to next vector
                        floatPointer += floatStride;
                    }
                    //std::cout << "}\n";
                }
                else if (gattrib.first.compare("NORMAL") == 0)
                {
                    normalStream = MeshStream::create("i_" + MeshStream::NORMAL_ATTRIBUTE, MeshStream::NORMAL_ATTRIBUTE, 0);

                    // Fill in normal stream 
                    MeshFloatBuffer& buffer = normalStream->getData();

                    // Resize data and get pointer to data as float. 
                    size_t dataCount = gAccessor.count;
                    //buffer.resize(dataCount * vectorSize);

                    const unsigned char* charPointer = &(gBuffer.data[0]);
                    charPointer += byteOffset;
                    float* floatPointer = (float*)charPointer;
                    //std::cout << "{\n";
                    for (size_t i = 0; i < dataCount; i++)
                    {
                        //std::cout << "[" + std::to_string(i) + "] = { ";
						for (size_t v = 0; v < vectorSize; v++)
                        {
                            // Jump to component in vector and save
                            float bufferData = *(floatPointer + v);
                            buffer.push_back(bufferData);
                            //std::cout << std::to_string(bufferData) + " ";
                        }
                        //std::cout << " }" << std::endl;

                        // Jump to start of vector
                        floatPointer += floatStride;
                    }
                    //std::cout << "}\n";
                }
                else if (gattrib.first.compare("TEXCOORD_0") == 0)
                {
                    if (!texcoordStream)
                    {
                        texcoordStream = MeshStream::create("i_" + MeshStream::TEXCOORD_ATTRIBUTE + "_0", MeshStream::TEXCOORD_ATTRIBUTE, 0);
                    }

                    if (vectorSize == 2)
                    {
                        texcoordStream->setStride(MeshStream::STRIDE_2D);
                    }

                    // Fill in texture coordinate stream 
                    MeshFloatBuffer& buffer = texcoordStream->getData();

                    // Resize data and get pointer to data as float. 
                    size_t dataCount = gAccessor.count;
                    //buffer.resize(dataCount* vectorSize);

                    const unsigned char* charPointer = &(gBuffer.data[0]);
                    charPointer += byteOffset;
                    float* floatPointer = (float*)charPointer;
                    //std::cout << "{\n";
                    for (size_t i = 0; i < dataCount; i++)
                    {
                        //std::cout << "[" + std::to_string(i) + "] = { ";
                        for (size_t v = 0; v < vectorSize; v++)
                        {
                            // Jump to next vector 
                            float bufferData = *( floatPointer + v);
                            buffer.push_back(bufferData);
                            //std::cout << std::to_string(bufferData) + " ";
                        }
                        //std::cout << " }" << std::endl;

                        // Jump to start of vector
                        floatPointer += floatStride;
                    }
                    //std::cout << "}\n";
                }
                else if (gattrib.first.compare("TANGENT") == 0)
                {
                    tangentStream = MeshStream::create("i_" + MeshStream::TANGENT_ATTRIBUTE, MeshStream::NORMAL_ATTRIBUTE, 0);

                    // Fill in tangent stream 
                    MeshFloatBuffer& buffer = tangentStream->getData();

                    // Resize data and get pointer to data as float. 
                    size_t dataCount = gAccessor.count;
                    if (vectorSize > 3)
                    {
                        vectorSize = 3;
                    }
                    //buffer.resize(dataCount * vectorSize);

                    const unsigned char* charPointer = &(gBuffer.data[0]);
                    charPointer += byteOffset;
                    float* floatPointer = (float*)charPointer;
                    //std::cout << "{\n";
                    for (size_t i = 0; i < dataCount; i++)
                    {
                        //std::cout << "[" + std::to_string(i) + "] = { ";
                        for (size_t v = 0; v < vectorSize; v++)
                        {
                            // Jump to component in vector and save
                            float bufferData = *(floatPointer + v);
                            buffer.push_back(bufferData);
                            //std::cout << std::to_string(bufferData) + " ";
                        }
                        //std::cout << " }" << std::endl;

                        // Jump to next vector
                        floatPointer += floatStride;
                    }
                    //std::cout << "}\n";
                }
            }
        }

        // General noramsl if none provided
        if (!normalStream && positionStream)
        {
            normalStream = mesh->generateNormals(positionStream);
        }

        // Generate tangents if none provided
        if (!tangentStream && texcoordStream && positionStream && normalStream)
        {
            tangentStream = mesh->generateTangents(positionStream, normalStream, texcoordStream);
        }

        // Assign streams to mesh.
        if (positionStream)
        {
            mesh->addStream(positionStream);
        }
        if (normalStream)
        {
            mesh->addStream(normalStream);
        }
        if (texcoordStream)
        {
            mesh->addStream(texcoordStream);
        }
        if (tangentStream)
        {
            mesh->addStream(tangentStream);
        }

        // Assign properties to mesh.
        if (positionStream)
        {
            mesh->setVertexCount(positionStream->getData().size() / MeshStream::STRIDE_3D);
        }
        mesh->setMinimumBounds(boxMin);
        mesh->setMaximumBounds(boxMax);
        Vector3 sphereCenter = (boxMax + boxMin) * 0.5;
        mesh->setSphereCenter(sphereCenter);
        mesh->setSphereRadius((sphereCenter - boxMin).getMagnitude());
    }
	return true;
}

} // namespace MaterialX
