#pragma once

#include "assimp/scene.h"
#include "shader.h"
#include <string>
#include <vector>

namespace rendeer
{
    class Mesh;
    struct TextureData;
    class Model
    {
    public: 
        Model(std::string path);
        ~Model();

        void Draw(Shader *shader);
    private:
        std::vector<Mesh> meshes;
        std::string directory;

        void loadModel(std::string path);
        void processNode(aiNode *node, const aiScene *scene);
        Mesh processMesh(aiMesh *mesh, const aiScene *scene);
        std::vector<TextureData> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);
    };
} // namespace rendeer
