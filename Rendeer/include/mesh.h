#pragma once

namespace rendeer
{
    class Mesh
    {
    public:
        Mesh(const float* vertices, const int dimension);
        Mesh(const float* vertices, const int* indices);
        ~Mesh();

        void Bind();
        void Unbind();

    private:
        unsigned int mVbo, mVao, mEbo;
        unsigned int mVertexCount, mElementCount;
    };
}