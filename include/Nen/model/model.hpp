#pragma once
#include "../Physics/Collision.hpp"
#include "../Vertex/VertexArray.hpp"
#include <string_view>

namespace nen {
class model {
public:
  model() = default;
  ~model() = default;

  void load(std::string_view str, std::string_view name);
  nen::aabb m_aabb;

private:
  nen::vertex_array m_array;
};
} // namespace nen