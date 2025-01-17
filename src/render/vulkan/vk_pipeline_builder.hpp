#ifndef VK_PIPELINE_BUILDER_HPP
#define VK_PIPELINE_BUILDER_HPP
#include "vk_depth_texture.hpp"
#include "vk_pipeline.hpp"
#include "vk_render_texture.hpp"

namespace sinen {
class vk_pipeline_builder {
public:
  vk_pipeline_builder(VkDevice device,
                      vk_pipeline_layout &pipeline_layout_instance,
                      vk_pipeline_layout &pipeline_layout_normal,
                      vk_pipeline_layout &pipeline_layout_depth_instance,
                      vk_pipeline_layout &pipeline_layout_depth,
                      vk_render_texture &render_texture,
                      vk_render_texture &present_texture,
                      vk_depth_texture &depth_texture,
                      VkRenderPass &render_pass, bool is_MSAA)
      : device(device), pipeline_layout_instance(pipeline_layout_instance),
        pipeline_layout_normal(pipeline_layout_normal),
        pipeline_layout_depth_instance(pipeline_layout_depth_instance),
        pipeline_layout_depth(pipeline_layout_depth),
        render_texture(render_texture), present_texture(present_texture),
        depth_texture(depth_texture), render_pass(render_pass),
        is_MSAA(is_MSAA) {}
  void skybox(vk_pipeline &pipeline);
  void opaque(vk_pipeline &pipeline);
  void alpha(vk_pipeline &pipeline);
  void alpha_2d(vk_pipeline &pipeline);
  void instancing_opaque(vk_pipeline &pipeline);
  void instancing_alpha(vk_pipeline &pipeline);
  void instancing_alpha_2d(vk_pipeline &pipeline);
  void depth(vk_pipeline &pipeline);
  void depth_instancing(vk_pipeline &pipeline);
  void ui(vk_pipeline &pipeline);
  void render_texture_pipeline(vk_pipeline &pipeline);
  void present_texture_pipeline(vk_pipeline &pipeline);

private:
  VkDevice device;
  vk_pipeline_layout &pipeline_layout_instance;
  vk_pipeline_layout &pipeline_layout_normal;
  vk_pipeline_layout &pipeline_layout_depth_instance;
  vk_pipeline_layout &pipeline_layout_depth;
  vk_render_texture &render_texture;
  vk_render_texture &present_texture;
  vk_depth_texture &depth_texture;
  VkRenderPass &render_pass;
  bool is_MSAA;
};

} // namespace sinen
#endif // VK_PIPELINE_BUILDER_HPP