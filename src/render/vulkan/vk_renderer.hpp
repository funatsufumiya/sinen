﻿#pragma once
#include "instancing/instancing.hpp"
#include <functional>
#include <memory>
#if !defined(EMSCRIPTEN) && !defined(MOBILE)

#include "vk_base.hpp"
#include "vk_pipeline.hpp"
#include "vk_pipeline_layout.hpp"
#include <array>
#include <draw_object/draw_object.hpp>
#include <string_view>
#include <unordered_map>
#include <vk_mem_alloc.h>

namespace nen {
struct vk_buffer_object {
  VkBuffer buffer;
  VmaAllocation allocation;
};
struct vk_image_object {
  VkImage image;
  VmaAllocation allocation;
  VkImageView view;
};

class vk_draw_object {
public:
  std::vector<VkDescriptorSet> descripterSet;
  std::vector<vk_buffer_object> uniformBuffers;
  bool isInstance = false;
  std::shared_ptr<nen::draw_object> drawObject;
};

struct vk_vertex_array : public vertex_array {
  vk_buffer_object vertexBuffer;
  vk_buffer_object indexBuffer;
};

class vk_instancing {
public:
  vk_instancing(const instancing &_instancing) : ins(_instancing) {}
  instancing ins;
  std::shared_ptr<class vk_draw_object> m_vk_draw_object;
  vk_buffer_object instance_buffer;
};

class vk_renderer {
public:
  vk_renderer();
  ~vk_renderer();
  void initialize();
  void shutdown();
  void render();

  void add_vertex_array(const vertex_array &vArray, std::string_view name);
  void update_vertex_array(const vertex_array &vArray, std::string_view name);

  void draw2d(std::shared_ptr<class draw_object> sprite);
  void draw3d(std::shared_ptr<class draw_object> sprite);

  void load_shader(const shader &shaderInfo);
  void unload_shader(const shader &shaderInfo);

  void add_instancing(const instancing &_instancing);

  void prepare();
  void cleanup();
  void make_command(VkCommandBuffer command, VkRenderPassBeginInfo &ri,
                    VkCommandBufferBeginInfo &ci, VkFence &fence);
  void draw3d(VkCommandBuffer);
  void draw2d(VkCommandBuffer);
  vk_buffer_object create_buffer(
      uint32_t size, VkBufferUsageFlags usage,
      VkMemoryPropertyFlags flags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
  vk_base *GetBase() { return m_base.get(); }
  void registerTexture(std::shared_ptr<class vk_draw_object> texture,
                       texture_type type);
  void unregisterTexture(std::shared_ptr<class vk_draw_object> texture);
  void register_image_object(const handle_t &handle);
  void unregister_image_object(const handle_t &handle);
  VkPipelineLayout get_pipeline_layout(const std::string &name) {
    return m_pipeline_layout.GetLayout();
  }
  VkDescriptorSetLayout get_descriptor_set_layout(const std::string &name) {
    return m_descriptor_set_layout;
  }
  VkRenderPass get_render_pass(const std::string &name);
  VkDescriptorPool get_descriptor_pool() const { return m_descriptor_pool; }
  VkDevice get_device();
  uint32_t get_memory_type_index(uint32_t requestBits,
                                 VkMemoryPropertyFlags requestProps) const;
  void destroy_buffer(vk_buffer_object &bufferObj);
  void destroy_image(vk_image_object &imageObj);
  void write_memory(VmaAllocation, const void *data, size_t size);

  VmaAllocator allocator{};

private:
  std::unique_ptr<class vk_base> m_base;
  void prepare_descriptor_set_layout();
  void prepare_descriptor_pool();
  void prepare_descriptor_set(std::shared_ptr<vk_draw_object>);
  void prepare_imgui();
  void render_imgui(VkCommandBuffer command);
  void draw_skybox(VkCommandBuffer command);
  void draw_instancing_3d(VkCommandBuffer command);
  void draw_instancing_2d(VkCommandBuffer command);
  void update_image_object(const handle_t &handle);
  VkSampler create_sampler();
  vk_image_object create_texture(SDL_Surface *imagedata, VkFormat format);
  vk_image_object create_texture_from_surface(const ::SDL_Surface &surface);
  vk_image_object
  create_texture_from_memory(const std::vector<char> &imageData);
  void set_image_memory_barrier(VkCommandBuffer command, VkImage image,
                                VkImageLayout oldLayout,
                                VkImageLayout newLayout);
  std::unordered_map<std::string, vk_vertex_array> m_vertex_arrays;
  std::vector<VkDescriptorSetLayout> layouts;
  VkDescriptorSetLayout m_descriptor_set_layout;
  VkDescriptorPool m_descriptor_pool;
  VkSampler m_sampler;
  VkPhysicalDeviceMemoryProperties m_physical_mem_props;
  vk_pipeline_layout m_pipeline_layout;
  vk_pipeline pipeline_skybox;
  vk_pipeline pipeline_opaque;
  vk_pipeline pipeline_alpha;
  vk_pipeline pipeline_2d;
  vk_pipeline pipeline_instancing_opaque;
  vk_pipeline pipeline_instancing_alpha;
  vk_pipeline pipeline_instancing_2d;
  std::vector<std::pair<shader, vk_pipeline>> m_user_pipelines;
  std::vector<std::shared_ptr<vk_draw_object>> m_draw_object_3d;
  std::vector<std::shared_ptr<vk_draw_object>> m_draw_object_2d;
  std::unordered_map<handle_t, vk_image_object> m_image_object;
  std::vector<vk_instancing> m_instancies_3d;
  std::vector<vk_instancing> m_instancies_2d;
  vk_buffer_object m_instance_buffer;
};
} // namespace nen
#endif
