#include "mesh.h"
#include "glad/glad.h"
#include <array>

namespace rendeer
{
    Mesh::Mesh(const float *vertices, const int dimension, const int *attributes, const int numAttributes) : mVao(0), mVbo(0), mEbo(0)
    {
        glGenVertexArrays(1, &mVao);
        glBindVertexArray(mVao);

        glGenBuffers(1, &mVbo);
        glBindBuffer(GL_ARRAY_BUFFER, mVbo);
        glBufferData(GL_ARRAY_BUFFER, dimension, vertices, GL_STATIC_DRAW);

        int stride{0}, offset{0};
        for (int i = 0; i < numAttributes; i++) stride += attributes[i];

        for (int i = 0; i < numAttributes; i++)
        {
            glEnableVertexAttribArray(i);
            glVertexAttribPointer(i, attributes[i], GL_FLOAT, GL_FALSE, stride * sizeof(float), (void *) (offset * sizeof(float)));
            offset += attributes[i];
        }
        
        glBindVertexArray(0);
    }

    // Mesh::Mesh(const float *vertices, const int *indices) 
    // {
    //     glBindVertexArray(mVao);

    //     glGenBuffers(1, &mEbo);
    //     glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEbo);
    //     glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    //     glBindVertexArray(0);
    // }

    Mesh::~Mesh()
    {
        glDeleteVertexArrays(1, &mVao);
        glDeleteBuffers(1, &mVbo);
    }

    void Mesh::Bind()
    {
        glBindVertexArray(mVao);
    }

    void Mesh::Unbind()
    {
        glBindVertexArray(0);
    }
} // namespace rendeer
