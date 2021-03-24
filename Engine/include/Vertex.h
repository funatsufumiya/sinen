#pragma once
#include <Engine/include/Math.hpp>
#include <array>
namespace nen
{
    struct Vertex
    {
        Vector3f pos;
        std::array<float, 3> norm;
        Vector2f uv;
    };
}