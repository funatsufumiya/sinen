#ifndef SINEN_DRAWABLE_HPP
#define SINEN_DRAWABLE_HPP
#include "../math/vector2.hpp"
#include "../shader/shader.hpp"
#include "../texture/texture.hpp"
#include "../utility/handler.hpp"

namespace sinen {
/**
 * @brief Drawable object class
 *
 */
struct drawable {
  /**
   * @brief Construct a new draw object object
   *
   */
  drawable() = default;
  /**
   * @brief Destroy the draw object object
   *
   */
  ~drawable() = default;
  /**
   * @brief Shader parameter
   *
   */
  struct parameter {
    matrix4 world;
    matrix4 view;
    matrix4 proj;
  };
  // parameter
  parameter param;
  /**
   * @brief Shader data
   *
   */
  shader shader_data;
  /**
   * @brief Use texture
   *
   */
  texture texture_handle;
  /**
   * @brief Vertex index
   *
   */
  std::string vertexIndex;
  bool is_draw_depth = true;
  int drawOrder = 100;
  int nodeNum = 1;
};

} // namespace sinen
#endif // !SINEN_DRAWABLE_HPP
