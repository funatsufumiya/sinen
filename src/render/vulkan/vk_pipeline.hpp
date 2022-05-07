#pragma once
#if !defined(EMSCRIPTEN) && !defined(MOBILE)
#include "vk_pipeline_layout.hpp"
#include <SDL_vulkan.h>
#include <vector>
#include <vulkan/vk_layer.h>

namespace nen {
class vk_pipeline {
public:
  vk_pipeline() = default;
  void Initialize(vk_pipeline_layout &layout, VkRenderPass renderPass,
                  std::vector<VkPipelineShaderStageCreateInfo> &shaderStages);
  void Prepare(VkDevice device);
  void Bind(VkCommandBuffer command);
  void Cleanup(VkDevice device);

  void SetDepthTest(VkBool32 isEnable);
  void SetDepthWrite(VkBool32 isEnable);
  void ColorBlendFactor(VkBlendFactor src, VkBlendFactor dst);
  void AlphaBlendFactor(VkBlendFactor src, VkBlendFactor dst);

private:
  VkPipeline pipeline;
  VkPipelineColorBlendStateCreateInfo cbCI;
  VkPipelineColorBlendAttachmentState blendAttachment;
  VkPipelineDepthStencilStateCreateInfo depthStencilCI;
  VkGraphicsPipelineCreateInfo graphicsCI;
};
} // namespace nen
#endif