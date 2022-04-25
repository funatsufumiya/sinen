# sinen
  sinen is a media library for C++20 that is being produced mainly for game development.  
The library is planned to be used only by me, so I don't think it will have full support and documentation.

## Supported platforms
[![Windows(MinGW)](https://github.com/astomih/sinen/actions/workflows/mingw.yml/badge.svg)](https://github.com/astomih/sinen/actions/workflows/mingw.yml)
[![Linux](https://github.com/astomih/sinen/actions/workflows/linux.yml/badge.svg)](https://github.com/astomih/sinen/actions/workflows/linux.yml)
[![Emscripten(Web)](https://github.com/astomih/sinen/actions/workflows/emscripten.yml/badge.svg)](https://github.com/astomih/sinen/actions/workflows/emscripten.yml)
[![Android](https://github.com/astomih/sinen/actions/workflows/android.yml/badge.svg)](https://github.com/astomih/sinen/actions/workflows/android.yml)

## Documents
[![doxygen](https://github.com/astomih/sinen/actions/workflows/doxygen.yml/badge.svg)](https://github.com/astomih/sinen/actions/workflows/doxygen.yml)  
[API references](https://astomih.github.io/sinen)  
No tutorials, etc. have been created yet.

## Hello world in sinen
``` lua
local hello_texture = {}
local hello_font = {}
local hello_drawer = {}

function setup()
	hello_texture = texture()
	hello_drawer = draw2d(hello_texture)
	hello_drawer.scale = vector2(1, 1)
	hello_font = font()
	hello_font:load(DEFAULT_FONT, 128)
	hello_font:render_text(hello_texture, "Hello Sinen World!", color(1, 1, 1, 1))
end

function update() 
	hello_drawer:draw()
end
```

## How to build
### Windows
Install msys, CMake and Vulkan. then install 'Using libraries' in pacman.  
Finally, build with CMake.
### WebGL
Install emscripten SDK. and do this.  
 ``` 
 $ emcmake cmake .
 $ emmake make
 ```
 ### Android
 Build with gradle.

## Dependencies 
- GLEW
- ImGui
- ImGuiColorTextEdit
- Lua
- Sol2
- mojoAL
- OpenGL 3.3
- SDL2(image,mixer,net,ttf)
- Vulkan 1.2
- VulkanMemoryAllocator
  
## Lisence
 MIT Lisence