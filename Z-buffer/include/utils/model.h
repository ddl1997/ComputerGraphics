#ifndef MODEL_H
#define MODEL_H

#include <Eigen/Dense>
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

struct vertex {
    Eigen::Vector3f Position;
    Eigen::Vector3f Normal;
    Eigen::Vector2f TexCoords;
};

struct face {
    std::vector<vertex> vertices;
    int edgeSize = 0;
};

struct texture {
    unsigned int id;
    std::string type;
};

class Mesh {
public:
    /*  网格数据  */
    std::vector<face> faces;
    std::vector<texture> textures;
    /*  函数  */
    Mesh(std::vector<face> faces, std::vector<texture> textures)
    {
        this->faces = faces;
        this->textures = textures;
    }
};

class Model
{
public:
    /*  模型数据  */
    std::vector<Mesh> meshes;
    std::string directory;

    Model(char* path)
    {
        loadModel(path);
    }
private:
    void loadModel(std::string path)
    {
        Assimp::Importer import;
        const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        {
            return;
        }
        directory = path.substr(0, path.find_last_of('/'));

        processNode(scene->mRootNode, scene);
    }

    void processNode(aiNode* node, const aiScene* scene)
    {
        // 处理节点所有的网格（如果有的话）
        for (unsigned int i = 0; i < node->mNumMeshes; i++)
        {
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            meshes.push_back(processMesh(mesh, scene));
        }
        // 接下来对它的子节点重复这一过程
        for (unsigned int i = 0; i < node->mNumChildren; i++)
        {
            processNode(node->mChildren[i], scene);
        }
    }

    Mesh processMesh(aiMesh* mesh, const aiScene* scene)
    {
        std::vector<vertex> vertices;
        std::vector<face> faces;
        std::vector<texture> textures;

        for (unsigned int i = 0; i < mesh->mNumVertices; i++)
        {
            vertex vertex;
            // 处理顶点位置、法线和纹理坐标
            Eigen::Vector3f pos(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
            vertex.Position = pos;
            Eigen::Vector3f norm(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
            vertex.Normal = norm;
            if (mesh->mTextureCoords[0]) // 网格是否有纹理坐标？
            {
                Eigen::Vector2f tex(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
                vertex.TexCoords = tex;
            }
            else
                vertex.TexCoords = Eigen::Vector2f(0.0f, 0.0f);

            vertices.push_back(vertex);
        }

        // 处理索引
        for (unsigned int i = 0; i < mesh->mNumFaces; i++)
        {
            aiFace mFace = mesh->mFaces[i];
            face f;

            for (unsigned int j = 0; j < mFace.mNumIndices; j++)
            {
                f.vertices.push_back(vertices[mFace.mIndices[j]]);
            }

            f.edgeSize = f.vertices.size();
            faces.push_back(f);
        }

        // 处理材质
        if (mesh->mMaterialIndex >= 0)
        {
            aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
            std::vector<texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
            textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
            std::vector<texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
            textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
        }

        return Mesh(faces, textures);
    }

    std::vector<texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
    {
        std::vector<texture> textures;
        for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
        {
            /*aiString str;
            mat->GetTexture(type, i, &str);
            texture texture;
            texture.id = TextureFromFile(str.C_Str(), directory);
            texture.type = typeName;
            texture.path = str;
            textures.push_back(texture);*/
        }
        return textures;
    }
};
#endif // !MODEL_H
