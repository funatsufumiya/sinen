#include "vk_pipeline.hpp"
#include "vk_renderer.hpp"
#include "vk_util.hpp"
#include "vulkan/vulkan_core.h"
#include <cstddef>
namespace sinen {
void vk_pipeline_layout::initialize(
    VkDevice device, const VkDescriptorSetLayout *descriptorLayout,
    const VkExtent2D &extent, bool instance) {
  // Setting vertex inputs
  if (instance) {
    vibDesc = {{
        // position 3, normal 3, uv 2, color4 = 12
        {0, sizeof(Vertex), VK_VERTEX_INPUT_RATE_VERTEX},
        {1, sizeof(InstanceData), VK_VERTEX_INPUT_RATE_INSTANCE},
    }};

    inputAttribs = {{
        {0, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, position)},
        {1, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, normal)},
        {2, 0, VK_FORMAT_R32G32_SFLOAT, offsetof(Vertex, uv)},
        {3, 0, VK_FORMAT_R32G32B32A32_SFLOAT, offsetof(Vertex, rgba)},
        {4, 1, VK_FORMAT_R32G32B32A32_SFLOAT,
         offsetof(InstanceData, world_matrix_1)},
        {5, 1, VK_FORMAT_R32G32B32A32_SFLOAT,
         offsetof(InstanceData, world_matrix_2)},
        {6, 1, VK_FORMAT_R32G32B32A32_SFLOAT,
         offsetof(InstanceData, world_matrix_3)},
        {7, 1, VK_FORMAT_R32G32B32A32_SFLOAT,
         offsetof(InstanceData, world_matrix_4)},
    }};
  } else {
    vibDesc = {{// position 3, normal 3, uv 2, color4 = 12
                {0, sizeof(Vertex), VK_VERTEX_INPUT_RATE_VERTEX}}};

    inputAttribs = {
        {{0, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, position)},
         {1, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, normal)},
         {2, 0, VK_FORMAT_R32G32_SFLOAT, offsetof(Vertex, uv)},
         {3, 0, VK_FORMAT_R32G32B32A32_SFLOAT, offsetof(Vertex, rgba)}}};
  }

  vertexInputCI = VkPipelineVertexInputStateCreateInfo{
      VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
      nullptr,
      0,
      uint32_t(vibDesc.size()),
      vibDesc.data(),
      uint32_t(inputAttribs.size()),
      inputAttribs.data(),
  };

  // Setting blending
  colorMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
              VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;

  blendAttachment = VkPipelineColorBlendAttachmentState{};
  blendAttachment.blendEnable = VK_FALSE;
  blendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
  blendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
  blendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
  blendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
  blendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
  blendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;
  blendAttachment.colorWriteMask = colorMask;

  cbCI = VkPipelineColorBlendStateCreateInfo{};
  cbCI.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
  cbCI.attachmentCount = 1;
  cbCI.pAttachments = &blendAttachment;
  // setup dynamic state
  dynamicStates.push_back(VK_DYNAMIC_STATE_SCISSOR);
  dynamicStates.push_back(VK_DYNAMIC_STATE_VIEWPORT);
  pipelineDynamicStateCI = {
      VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO, nullptr, 0,
      uint32_t(dynamicStates.size()), dynamicStates.data()};

  // Set viewport
  viewport = VkViewport{.x = 0.f,
                        .y = static_cast<float>(extent.height),
                        .width = static_cast<float>(extent.width),
                        .height = -static_cast<float>(extent.height),
                        .minDepth = 0.f,
                        .maxDepth = 1.f};
  scissor = {
      .offset = {0, 0},
      .extent = extent,
  };

  viewportCI = VkPipelineViewportStateCreateInfo{
      .sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
      .pNext = nullptr,
      .flags = 0,
      .viewportCount = 1,
      .pViewports = &viewport,
      .scissorCount = 1,
      .pScissors = &scissor,
  };

  // Setting primitive toporogy
  inputAssemblyCI = VkPipelineInputAssemblyStateCreateInfo{};
  inputAssemblyCI.sType =
      VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
  inputAssemblyCI.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;

  // Setting rasterizer state
  rasterizerCI = VkPipelineRasterizationStateCreateInfo{};
  rasterizerCI.sType =
      VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
  rasterizerCI.polygonMode = VK_POLYGON_MODE_FILL;
  rasterizerCI.cullMode = VK_CULL_MODE_NONE;
  rasterizerCI.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
  rasterizerCI.lineWidth = 1.0f;

  // Setting depth stencil state
  depthStencilCI = VkPipelineDepthStencilStateCreateInfo{};
  depthStencilCI.sType =
      VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
  depthStencilCI.depthTestEnable = VK_TRUE;
  depthStencilCI.depthCompareOp = VK_COMPARE_OP_LESS_OR_EQUAL;
  depthStencilCI.depthWriteEnable = VK_TRUE;
  depthStencilCI.stencilTestEnable = VK_FALSE;

  // Setting pipeline layout
  pipelineLayoutCI = VkPipelineLayoutCreateInfo{};
  pipelineLayoutCI.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
  pipelineLayoutCI.setLayoutCount = 1;
  if (descriptorLayout)
    pipelineLayoutCI.pSetLayouts = descriptorLayout;
}

void vk_pipeline_layout::prepare(VkDevice device) {
  vkCreatePipelineLayout(device, &pipelineLayoutCI, NULL, &layout);
}

void vk_pipeline_layout::cleanup(VkDevice device) {
  vkutil::destroy_vulkan_object<VkPipelineLayout>(device, layout,
                                                  &vkDestroyPipelineLayout);
}
} // namespace sinen