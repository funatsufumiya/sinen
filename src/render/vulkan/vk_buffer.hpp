#ifndef SINEN_VK_BUFFER_HPP
#define SINEN_VK_BUFFER_HPP
#if !defined(EMSCRIPTEN) && !defined(ANDROID)
#include "vma.hpp"
namespace sinen {
struct vk_buffer {
  VkBuffer buffer;
  VmaAllocation allocation;
};
} // namespace sinen
#endif // !defined(EMSCRIPTEN) && !defined(MOBILE)
#endif // !SINEN_VK_BUFFER_HPP