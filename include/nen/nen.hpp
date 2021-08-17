#ifndef NEN_HPP
#define NEN_HPP
#ifdef ANDROID
#define MOBILE
#endif
#include "Math.hpp"
#include "Vertex.h"
#include "VertexArray.h"
#include "Vulkan/VKRenderer.h"
#include "Vulkan/Swapchain.h"
#include "Vulkan/VulkanShader.h"
#include "Vulkan/Pipeline.h"
#include "Vulkan/PipelineLayout.h"
#include "OpenGL/ShaderGL.h"
#include "OpenGL/GLRenderer.h"
#include "ES/ESRenderer.h"
#include "Random.h"
#include "Texture.h"
#include "Font.hpp"
#include "Window.hpp"
#include "Transform.hpp"
#include "Logger.h"
#include "AudioSystem.h"
#include "AudioEvent.h"
#include "Renderer.h"
#include "Transition.h"
#include "InputSystem.h"
#include "Effect.hpp"
#include "Script.h"
#include "ObjLoader.h"
#include "Vulkan/VulkanUtil.h"
#include "scene/Scene.hpp"
#include "actor/Actors.hpp"
#include "component/Components.hpp"
#endif
#ifdef NO_USE_NEN_NAMESPACE
using namespace nen;
#endif
