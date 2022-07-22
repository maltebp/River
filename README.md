# River
A hobby game framework  written in C++17

## Features
The framework is under constant development, but as of now the following features are included:

 - Dedicated ECS system (see [River ECS](https://github.com/maltebp/RiverECS))
 - Sprite animation
 - Rendering of textures/sprites
 - Rendering of variable sized texts using TrueType fonts
 - Core game loop management
 - Rendering/logic layers
 - Cameras
 - Keyboard and mouse events
 - Spatial Audio

## Technologies / libraries
The project makes use of following external libraries / technologies:

 - GLFW
 - OpenGL
 - OpenAL
 - FreeType
 - StbImage

## Usage

 1. Link against all the static libraries in `build/bin/River/<configuration>/lib`
 2. Install shared libraries (`.dll`s):
    - OpenGL
    - OpenAL
 3. Includes be found in `build/bin/River/<configuration/include`