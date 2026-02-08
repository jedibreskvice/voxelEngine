#include "Model.h"
#include <iostream>
#include <glad/glad.h>
#include "stb_image.h"

unsigned int TextureFromFile(const char* path, const std::string& directory)
{
    std::string filename = directory + '/' + std::string(path);

    std::cout << "Loading texture from: " << filename << std::endl;

    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << filename << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}

Model::Model(std::string const& path, bool gamma) : gammaCorrection(gamma)
{
    loadModel(path);
}

void Model::Draw(Shader& shader, bool outlines, glm::vec3 position, glm::vec3 scale, glm::vec3 rotation, int team, bool selected, float alphaValue)
{
    // Enable blending for transparency
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Create the model matrix
    glm::mat4 model = glm::mat4(1.0f); // Start with the identity matrix
    model = glm::translate(model, position); // Apply translation
    model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f)); // Rotate around x-axis
    model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f)); // Rotate around y-axis
    model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f)); // Rotate around z-axis
    model = glm::scale(model, scale); // Apply scaling

    // Pass the model matrix to the shader
    shader.setMat4("model", model);

    if (outlines) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        bool notMap = true;
        shader.setBool("notMap", notMap);

        for (unsigned int i = 0; i < meshes.size(); i++)
        {
            bool hasTexture = !meshes[i].textures.empty();
            shader.setBool("hasTexture", hasTexture);

            if (!hasTexture) {
                // Set the color for the model if no texture is applied
                glm::vec4 mc;
                if (team == 0) {
                    mc = glm::vec4(0.5f, 0.5f, 0.5f, alphaValue);
                } 
                else if (team == 1) {
                    mc = glm::vec4(0.1f, 0.62f, 1.0f, alphaValue);
                } 
                else if (team == 2) {
                    mc = glm::vec4(0.82f, 0.3f, 0.3f, alphaValue);
                }
                else if(team == 3) {
                    mc = glm::vec4(0.25f, 0.61f, 0.25f, alphaValue);
                }
                else if (team == 4) {
                    mc = glm::vec4(0.83f, 0.59f, 0.17f, alphaValue);
                }
                else if (team == 5) {
                    mc = glm::vec4(0.8f, 0.8f, 0.2f, alphaValue);
                }
                else if (team == 6) {
                    mc = glm::vec4(0.6f, 0.42f, 0.82f, alphaValue);
                }
                shader.setVec4("modelColor", mc.x, mc.y, mc.z, mc.w);
            }
            meshes[i].Draw(shader);
        }

        if (!selected) {
            glLineWidth(2.0f);
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            for (unsigned int i = 0; i < meshes.size(); i++)
            {
                bool hasTexture = !meshes[i].textures.empty();
                shader.setBool("hasTexture", hasTexture);

                if (!hasTexture) {
                    // Set the color for the model if no texture is applied
                    glm::vec4 mc = glm::vec4(0.1f, 0.1f, 0.1f, alphaValue);
                    shader.setVec4("modelColor", mc.x, mc.y, mc.z, mc.w);
                }
                meshes[i].Draw(shader);
            }
        }
        else {
            glLineWidth(4.0f);
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            for (unsigned int i = 0; i < meshes.size(); i++)
            {
                bool hasTexture = !meshes[i].textures.empty();
                shader.setBool("hasTexture", hasTexture);

                if (!hasTexture) {
                    // Set the color for the model if no texture is applied
                    glm::vec4 mc = glm::vec4(0.9f, 0.9f, 0.9f, alphaValue);
                    shader.setVec4("modelColor", mc.x, mc.y, mc.z, mc.w);
                }
                meshes[i].Draw(shader);
            }
        }

        // Reset to default polygon mode
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
    else {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        bool notMap = true;
        shader.setBool("notMap", notMap);

        for (unsigned int i = 0; i < meshes.size(); i++)
        {
            bool hasTexture = !meshes[i].textures.empty();
            shader.setBool("hasTexture", hasTexture);

            if (!hasTexture) {
                // Set the color for the model if no texture is applied
                glm::vec4 mc;
                if (team == 0) {
                    mc = glm::vec4(0.5f, 0.5f, 0.5f, alphaValue);
                }
                else if (team == 1) {
                    mc = glm::vec4(0.1f, 0.62f, 1.0f, alphaValue);
                }
                else if (team == 2) {
                    mc = glm::vec4(0.82f, 0.3f, 0.3f, alphaValue);
                }
                else if (team == 3) {
                    mc = glm::vec4(0.25f, 0.61f, 0.25f, alphaValue);
                }
                else if (team == 4) {
                    mc = glm::vec4(0.83f, 0.59f, 0.17f, alphaValue);
                }
                else if (team == 5) {
                    mc = glm::vec4(0.8f, 0.8f, 0.2f, alphaValue);
                }
                else if (team == 6) {
                    mc = glm::vec4(0.6f, 0.42f, 0.82f, alphaValue);
                }
                shader.setVec4("modelColor", mc.x, mc.y, mc.z, mc.w);
            }
            meshes[i].Draw(shader);
        }
    }
    glDisable(GL_BLEND);
}

void Model::loadModel(std::string const& path)
{
    // read file via ASSIMP
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
    // check for errors
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
    {
        std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
        return;
    }
    // retrieve the directory path of the filepath
    directory = path.substr(0, path.find_last_of('/'));

    // process ASSIMP's root node recursively
    processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode* node, const aiScene* scene)
{
    // process each mesh located at the current node
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        // the node object only contains indices to index the actual objects in the scene. 
        // the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh, scene));
    }
    // after we've processed all of the meshes (if any) we then recursively process each of the children nodes
    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene);
    }
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
    // data to fill
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    // walk through each of the mesh's vertices
    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;
        glm::vec3 vector; // we declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
        // positions
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.Position = vector;
        // normals
        vector.x = mesh->mNormals[i].x;
        vector.y = mesh->mNormals[i].y;
        vector.z = mesh->mNormals[i].z;
        vertex.Normal = vector;
        // texture coordinates
        if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
        {
            glm::vec2 vec;
            // vertices
            vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.TexCoords = vec;
        }
        else
            vertex.TexCoords = glm::vec2(0.0f, 0.0f);
        // tangent
        vector.x = mesh->mTangents[i].x;
        vector.y = mesh->mTangents[i].y;
        vector.z = mesh->mTangents[i].z;
        vertex.Tangent = vector;
        // bitangent
        vector.x = mesh->mBitangents[i].x;
        vector.y = mesh->mBitangents[i].y;
        vector.z = mesh->mBitangents[i].z;
        vertex.Bitangent = vector;

        vertices.push_back(vertex);
    }
    // now walk through each face (polygon) and retrieve the corresponding vertex indices.
    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        // retrieve all indices of the face and store them in the indices vector
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }
    // process materials
    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
    // get diffuse textures
    std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
    // get specular textures
    std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
    textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    // return a mesh object created from the extracted mesh data
    return Mesh(vertices, indices, textures);
}

std::vector<Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
{
    std::vector<Texture> textures;
    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);
        // check if texture was loaded before and if so, skip loading and just use the loaded texture.
        bool skip = false;
        for (unsigned int j = 0; j < textures_loaded.size(); j++)
        {
            if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0)
            {
                textures.push_back(textures_loaded[j]);
                skip = true; // a texture with the same path was loaded before, so we skip loading a new texture
                break;
            }
        }
        if (!skip)
        {   // if texture hasn't been loaded already, load it
            Texture texture;
            texture.id = TextureFromFile(str.C_Str(), directory);
            texture.type = typeName;
            texture.path = str.C_Str(); // store the path of the texture
            textures.push_back(texture);
            textures_loaded.push_back(texture); // add to loaded textures
        }
    }
    return textures;
}