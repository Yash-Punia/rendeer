#pragma once

#include "glm/glm.hpp"
#include "shader.h"

#include <string>
#include <vector>

namespace rendeer
{
    struct Vertex {
        glm::vec3 Position;
        glm::vec3 Normal;
        glm::vec2 TexCoords;
    };

    struct TextureData {
        unsigned int id;
        std::string type;
        std::string path;
    };

    class Mesh
    {
    public:
        Mesh(const float* vertices, const int dimension, const int* attributes, const int numAttributes);
        Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<TextureData> textures);
        // Mesh(const float* vertices, const int* indices);
        ~Mesh();

        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        std::vector<TextureData> textures;

        void Bind();
        void Unbind();
        void Draw(Shader *shader);
    private:
        unsigned int mVbo, mVao, mEbo;
        unsigned int mVertexCount, mElementCount;
    };
}