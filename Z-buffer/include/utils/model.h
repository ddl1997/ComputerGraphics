#ifndef MODEL_H
#define MODEL_H

#include <Eigen/Dense>
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "texture.h"
#include "global.h"

struct vertex {
    Eigen::Vector3f Position;
    Eigen::Vector3f Normal;
    Eigen::Vector2f TexCoords;
};

struct face {
    std::vector<vertex> vertices;
    int edgeSize = 0;
};

class Mesh {
public:
    std::vector<face> faces;
    std::vector<texture> textures;
    /*  ����  */
    Mesh(std::vector<face> faces, std::vector<texture> textures)
    {
        this->faces = faces;
        this->textures = textures;
    }
};

class Model
{
public:
    std::vector<Mesh> meshes;
    std::string directory;

    Model(std::string path)
    {
        loadModel(path);
    }
private:
    void loadModel(std::string path)
    {
        Assimp::Importer import;
        // ����ģ��ʱ�Ὣmesh��������Ƭת��Ϊ������
        const aiScene* scene = import.ReadFile(RESOURCEDIR + path, aiProcess_Triangulate | aiProcess_FlipUVs);

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        {
            return;
        }
        directory = path.substr(0, path.find_last_of('/'));

        //�ݹ�ش���mesh
        processNode(scene->mRootNode, scene);
    }

    void processNode(aiNode* node, const aiScene* scene)
    {
        // ����ڵ����е���������еĻ���
        for (unsigned int i = 0; i < node->mNumMeshes; i++)
        {
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            meshes.push_back(processMesh(mesh, scene));
        }
        // �������������ӽڵ��ظ���һ����
        for (unsigned int i = 0; i < node->mNumChildren; i++)
        {
            processNode(node->mChildren[i], scene);
        }
    }

    // ��ÿ��������mesh���д���
    Mesh processMesh(aiMesh* mesh, const aiScene* scene)
    {
        std::vector<vertex> vertices;
        std::vector<face> faces;
        std::vector<texture> textures;

        for (unsigned int i = 0; i < mesh->mNumVertices; i++)
        {
            vertex vertex;
            // ������λ�á����ߺ���������
            Eigen::Vector3f pos(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
            vertex.Position = pos;

            if (mesh->mNormals) // �����Ƿ��з�������
            {
                Eigen::Vector3f norm(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
                vertex.Normal = norm;
            }
            
            if (mesh->mTextureCoords[0]) // �����Ƿ����������ꣿ
            {
                Eigen::Vector2f tex(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
                vertex.TexCoords = tex;
            }
            else
                vertex.TexCoords = Eigen::Vector2f(0.0f, 0.0f);

            vertices.push_back(vertex);
        }

        // ������
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

        // �������
        /*if (mesh->mMaterialIndex >= 0)
        {
            aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
            std::vector<texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, TextureType::diffuse);
            textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
            std::vector<texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, TextureType::specular);
            textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
        }*/

        return Mesh(faces, textures);
    }

    // ����������ڴ��ڶ������ʱ����������أ������Ŀ�в�û���õ���������뺯��
    std::vector<texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, TextureType texType)
    {
        std::vector<texture> textures;
        for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
        {
            /*aiString str;
            mat->GetTexture(type, i, &str);
            texture texture(str.C_Str(), texType);
            textures.push_back(texture);*/
        }
        return textures;
    }
};
#endif // !MODEL_H
