#include <actor/actor.hpp>
#include <scene/scene.hpp>

namespace sinen {
actor::actor() : m_state(actor::state::active) {}
actor::~actor() {
  for (auto c : m_components) {
    delete c;
  }
  m_components.clear();
}
void actor::update(float delta_time) {
  for (auto &c : m_components) {
    c->update(delta_time);
  }
}
matrix4 actor::get_world_matrix() const {
  return matrix4::create_scale(m_scale) *
         matrix4::create_from_quaternion(quaternion::from_euler(m_rotation)) *
         matrix4::create_translation(m_position);
}

void actor::add_component(actor::component_ptr comp) {
  m_components.push_back(std::move(comp));
}
void actor::remove_component(actor::component_ptr comp) {
  for (auto itr = m_components.begin(); itr != m_components.end();) {
    if (*itr == comp) {
      m_components.erase(itr);
      delete comp;
      break;
    }
    itr++;
  }
}
} // namespace sinen
