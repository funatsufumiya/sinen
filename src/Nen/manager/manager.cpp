#include "Utility/Singleton.hpp"
#ifndef MOBILE
#define SDL_MAIN_HANDLED
#else
#include <SDL_main.h>
#endif
#include <Nen.hpp>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_net.h>
#include <SDL_ttf.h>
#include <fstream>
#include <memory>

#ifdef EMSCRIPTEN
#include <emscripten.h>
#endif

std::function<void()> loop;
std::function<void(std::unique_ptr<nen::base_scene>)> changeScene;
void main_loop() { loop(); }
namespace nen {
void ChangeScene(std::unique_ptr<base_scene> newScene) {
  changeScene(std::move(newScene));
}
void manager::launch(std::unique_ptr<base_scene> scene) {
  std::unique_ptr<nen::base_scene> nextScene;
  changeScene = [&](std::unique_ptr<nen::base_scene> newScene) {
    scene->Quit();
    nextScene = std::move(newScene);
  };
  SDL_SetMainReady();
  SDL_Init(SDL_INIT_EVERYTHING);
  TTF_Init();
  IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);
  SDLNet_Init();
  nextScene = nullptr;
  std::shared_ptr<nen::window> window = std::make_shared<nen::window>();
  nen::logger::MakeLogger(
      std::move(nen::logger::default_logger::CreateConsoleLogger()));
  std::shared_ptr<nen::renderer> renderer;
#if !defined(EMSCRIPTEN) && !defined(MOBILE)
  std::ifstream ifs("./api");
  std::string str;
  if (ifs.fail())
    throw std::runtime_error("api file not found.");
  std::getline(ifs, str);
  if (str.compare("Vulkan") == 0) {
    window->Initialize(nen::vector2(1280, 720), "Nen : Vulkan",
                       nen::graphics_api::Vulkan);
    renderer =
        std::make_shared<nen::renderer>(nen::graphics_api::Vulkan, window);
  } else if (str.compare("OpenGL") == 0) {
    window->Initialize(nen::vector2(1280, 720), "Nen : OpenGL",
                       nen::graphics_api::OpenGL);
    renderer =
        std::make_shared<nen::renderer>(nen::graphics_api::OpenGL, window);
  }

#else
  window->Initialize(nen::vector2(1280, 720), "Nen", nen::graphics_api::ES);
  renderer = std::make_shared<nen::renderer>(nen::graphics_api::ES, window);
#endif
  renderer->SetProjectionMatrix(nen::matrix4::Perspective(
      nen::Math::ToRadians(70.f), window->Size().x / window->Size().y, 0.1f,
      1000.f));

  auto soundSystem = std::make_shared<nen::sound_system>();
  if (!soundSystem->Initialize()) {
    nen::logger::Info("Failed to initialize audio system");
    soundSystem->Shutdown();
    soundSystem = nullptr;
    std::exit(-1);
  }
  nen::logger::Info("Audio system Initialized.");
  auto inputSystem = std::make_shared<nen::input_system>(*renderer);
  if (!inputSystem->Initialize()) {
    nen::logger::Info("Failed to initialize input system");
  }
  nen::logger::Info("Input system initialized.");
  // スクリプトのインスタンスを作成
  nen::script::Create();
  nen::logger::Info("Script system initialized.");
  scene->SetRenderer(renderer);
  scene->SetInputSystem(inputSystem);
  scene->SetSoundSystem(soundSystem);
  scene->Initialize();
  loop = [&] {
    if (scene->isRunning())
      scene->RunLoop();
    else if (nextScene) {
      scene->Shutdown();
      scene = std::move(nextScene);
      scene->SetRenderer(renderer);
      scene->SetInputSystem(inputSystem);
      scene->SetSoundSystem(soundSystem);
      scene->Initialize();
      nextScene = nullptr;
    } else {
      scene->Shutdown();
      inputSystem->Shutdown();
      soundSystem->Shutdown();
      scene = nullptr;
      renderer->Shutdown();
      renderer = nullptr;
      window = nullptr;
      singleton_finalizer::Finalize();
#ifndef EMSCRIPTEN
      std::exit(0);
#else
      emscripten_force_exit(0);
#endif
    }
  };

#if !defined(EMSCRIPTEN)
  while (true)
    loop();
#else
  emscripten_set_main_loop(main_loop, 120, true);
#endif
}

} // namespace nen