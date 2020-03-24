# SteelgearEngine2D
The SteelgearEngine2D is a library for creating 2D games in C++.

## Rendering
The api allows for the rendering of 2D shapes and textures by wrapping [SFML](https://www.sfml-dev.org/) and has several useful features.

### Customizable pipeline
The engine allows you to build your own pipeline using a commandbuffer system that takes inspiration from D3D12. It also allows for custom glsl shaders along with binding different typs of data to the shaders.

### Threaded rendering
It is possible to render either on a separate thread or on the same thread as is using the engine. The threading is done in a way so that no synchronization is needed from the user when it comes to transfering data to the render thread. Simply update the entities in the scene and execute the commandbuffer. The engine handles the rest.

## Scripting
Using the engine you can both expose functionality to and load and use scripts made in [AngelScript](https://www.angelcode.com/angelscript/).

## Input handling
By allowing the user to bind an identifier to an input source it becomes easy to handle both the input and keybindings. No need for hardcoding what the input source is everywhere. Instead, simply bind an identifier and then check for input using that identifier without having to know where it is coming from.
