#include "editor.hpp"
// imgui
#include <imgui.h>
#include <imgui_impl_sdl.h>
// Added for ImGui
#include <ImGuizmo.h>

#include "file_dialog.hpp"
#include "markdown.hpp"
#include "texteditor.hpp"
namespace sinen {
void imguizmo() {
  ImGui::Begin("ImGuizmo");
  ImGuizmo::BeginFrame();
  static ImGuizmo::OPERATION mCurrentGizmoOperation(ImGuizmo::ROTATE);
  static ImGuizmo::MODE mCurrentGizmoMode(ImGuizmo::WORLD);
  if (ImGui::IsKeyPressed(90))
    mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
  if (ImGui::IsKeyPressed(69))
    mCurrentGizmoOperation = ImGuizmo::ROTATE;
  if (ImGui::IsKeyPressed(82)) // r Key
    mCurrentGizmoOperation = ImGuizmo::SCALE;
  if (ImGui::RadioButton("Translate",
                         mCurrentGizmoOperation == ImGuizmo::TRANSLATE))
    mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
  ImGui::SameLine();
  if (ImGui::RadioButton("Rotate", mCurrentGizmoOperation == ImGuizmo::ROTATE))
    mCurrentGizmoOperation = ImGuizmo::ROTATE;
  ImGui::SameLine();
  if (ImGui::RadioButton("Scale", mCurrentGizmoOperation == ImGuizmo::SCALE))
    mCurrentGizmoOperation = ImGuizmo::SCALE;
  ImGuizmo::SetRect(0, 0, window::size().x, window::size().y);
  float mat[16];
  float deltam[16];
  matrix4 pos;
  pos[0][0] = 1.0f;
  pos[1][1] = 1.0f;
  pos[2][2] = 1.0f;
  pos[2][0] = 0.0f;
  pos[2][1] = 1.0f;
  pos[2][2] = 1.0f;
  ImGuizmo::DrawGrid(camera::view().get(), camera::projection().get(),
                     pos.get(), 20);
  ImGuizmo::DrawCubes(camera::view().get(), camera::projection().get(),
                      pos.get(), 16);
  ImGuizmo::Manipulate(camera::view().get(), camera::projection().get(),
                       mCurrentGizmoOperation, mCurrentGizmoMode, mat, deltam,
                       nullptr);
  static vector3 translation(0.0f, 0.0f, 10.0f);
  static vector3 target = vector3(0);
  static vector3 up = vector3(0.0f, -1.0f, 0.0f);
  ImGui::SliderFloat("X", &translation.x, -100.0f, 100.0f);
  ImGui::SliderFloat("Y", &translation.y, -100.0f, 100.0f);
  ImGui::SliderFloat("Z", &translation.z, -100.0f, 100.0f);
  ImGui::SliderFloat("tX", &target.x, -100.0f, 100.0f);
  ImGui::SliderFloat("tY", &target.y, -100.0f, 100.0f);
  ImGui::SliderFloat("tZ", &target.z, -100.0f, 100.0f);
  camera::lookat(translation, target, up);
  ImGui::End();
}

class editor::implements {
public:
  bool is_run = false;
  bool is_save = false;
};

editor::editor() : m_impl(std::make_unique<editor::implements>()) {}
editor::~editor() {}
void editor::setup() {
  m_impl->is_run = false;
  renderer::add_imgui_function(texteditor);
  renderer::add_imgui_function(markdown);
  renderer::add_imgui_function(imguizmo);
  // renderer::add_imgui_function(func_file_dialog);
  renderer::toggle_show_imgui();
}
void editor::update(float delta_time) {
  if (renderer::is_show_imgui() &&
      input::keyboard.get_key_state(key_code::F5) == button_state::Pressed) {
    m_impl->is_run = true;
  }
  if (m_impl->is_run) {
    m_impl->is_run = false;
  }
  if (renderer::is_show_imgui() &&
      input::keyboard.is_key_down(key_code::LCTRL) &&
      input::keyboard.get_key_state(key_code::S) == button_state::Pressed) {
    m_impl->is_save = true;
  }
  if (m_impl->is_save) {
    auto str = get_text();
    dstream::write(asset_type::Script,
                   main::get_current_scene_number() + ".lua", str);
    std::cout << str << std::endl;
    m_impl->is_save = false;
  }
}
} // namespace sinen