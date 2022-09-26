#pragma once

namespace rendeer
{
    class Mesh
    {
    public:
        Mesh();
        ~Mesh();

    private:
        unsigned int mVbo, mVao, mEbo;
        unsigned int mVertexCount, mElementCount;
    };
}