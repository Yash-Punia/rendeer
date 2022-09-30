#include "mesh.h"
#include "glad/glad.h"

namespace rendeer
{
    Mesh::Mesh(const float *vertices, const int dimension) : mVao(0), mVbo(0), mEbo(0)
    {
        glGenVertexArrays(1, &mVao);
        glBindVertexArray(mVao);

        glGenBuffers(1, &mVbo);
        glBindBuffer(GL_ARRAY_BUFFER, mVbo);
        glBufferData(GL_ARRAY_BUFFER, dimension, vertices, GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0);

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));

        glBindVertexArray(0);
    }

    Mesh::Mesh(const float *vertices, const int *indices) : Mesh(vertices, 0)
    {
        glBindVertexArray(mVao);

        glGenBuffers(1, &mEbo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEbo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        glBindVertexArray(0);
    }

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
