// internal
#include "../render/render_system.hpp"
#include <shader/shader.hpp>

namespace sinen {
shader::shader()
    : m_parameter(nullptr), m_parameter_size(0), m_vert_name("default"),
      m_frag_name("default") {}
shader::shader(std::string_view vertex_shader, std::string_view fragment_shader)
    : m_parameter(nullptr), m_parameter_size(0), m_vert_name(vertex_shader),
      m_frag_name(fragment_shader) {}
void shader::set_vertex_shader(std::string_view vertex_shader) {
  m_vert_name = vertex_shader;
}
void shader::set_fragment_shader(std::string_view fragment_shader) {
  m_frag_name = fragment_shader;
}
const std::string &shader::vertex_shader() const { return m_vert_name; }
const std::string &shader::fragment_shader() const { return m_frag_name; }

void shader::load() { render_system::load_shader(*this); }
void shader::unload() { render_system::unload_shader(*this); }
} // namespace sinen
