#pragma once
#if !defined(EMSCRIPTEN) && !defined(MOBILE)
#include <array>
#include <SDL_vulkan.h>
#include <vulkan/vk_layer.h>
#ifdef _WIN32
#include <vulkan/vulkan_win32.h>
#endif // _WIN32

namespace nen::vk
{
    class PipelineLayout
    {
    public:
        PipelineLayout() = default;
        void Initialize(VkDevice device, const VkDescriptorSetLayout *descriptorLayout, const VkExtent2D &extent);
        void Prepare(VkDevice device);
        void Cleanup(VkDevice device);
        const VkPipelineInputAssemblyStateCreateInfo* GetInputAssemblyCI() { return &inputAssemblyCI; }
        const VkPipelineViewportStateCreateInfo* GetViewportCI() { return &viewportCI; };
        const VkPipelineRasterizationStateCreateInfo* GetRasterizerCI() { return &rasterizerCI; };
        const VkPipelineMultisampleStateCreateInfo* GetMultisampleCI() { return &multisampleCI; };
        const VkPipelineDepthStencilStateCreateInfo* GetDepthStencilCI() { return &depthStencilCI; };
        const VkPipelineLayoutCreateInfo* GetPipelineLayoutCI() { return &pipelineLayoutCI; };
        const VkPipelineVertexInputStateCreateInfo* GetVertexInputCI() { return &vertexInputCI; };
        VkPipelineLayout GetLayout(){return layout;}

    private:
        VkPipelineLayout layout;
        VkVertexInputBindingDescription inputBinding;
        std::array<VkVertexInputBindingDescription, 2> vibDisc;
        std::array<VkVertexInputAttributeDescription, 3> inputAttribs;
        VkPipelineVertexInputStateCreateInfo vertexInputCI;
        int colorMask;
        VkPipelineColorBlendAttachmentState blendAttachment;
        VkPipelineColorBlendStateCreateInfo cbCI;
        VkViewport viewport;
        VkRect2D scissor;
        VkPipelineViewportStateCreateInfo viewportCI;
        VkPipelineInputAssemblyStateCreateInfo inputAssemblyCI;
        VkPipelineRasterizationStateCreateInfo rasterizerCI;
        VkPipelineMultisampleStateCreateInfo multisampleCI;
        VkPipelineDepthStencilStateCreateInfo depthStencilCI;
        VkPipelineLayoutCreateInfo pipelineLayoutCI;
    };
}
#endif