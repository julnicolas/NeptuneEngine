# NeptuneEngine

![Read Demo.jpg](https://github.com/Juli3nnicolas/NeptuneEngine/blob/master/Demo.png "Rendering demos")

Neptune is a multiplatform game-engine using OpenGL as rendering API. The engine currently runs on Windows but few work is necessary to port it on OSX and Linux.
Both 32-bit and 64-bit X86 architectures are supported. The engine has been succesfully tested on NVIDIA GeForce GT640M and Intel I7 (using the embedded GPU).

Neptune focuses more on 3D but is low-level enough to implement efficient 2D primitives.

At the moment the project only compiles on VS2013 but work is beeing done to simplify project build and deployment (first go using CMake at [TempNeptune](https://github.com/Juli3nnicolas/TempNeptune)).

To have some code examples (more end-to-end tests to be honest), please have a read to its companion repository [NeptunePracticeApp](https://github.com/Juli3nnicolas/NeptunePracticeApp).

## Features

At this point of the development, the engine supports the following features :
- Highly configurable GPU interface:
  - Possibility to simply run and chain shader programs
  - Possibility to send custom data to shaders with a simple API
  - Parameters can be simply updated
- Supports anti-aliasing with multi-sampling up to 16 times
- Deals with z-fighting issues for low-memory GPUs
- Loads a variety of 3D model formats (ply, obj, dae...)
- Loads a variety of texture formats (ktx, jpg, png)
- Supports 2D, 3D and Cube textures
- Supports Mipmaps
- Simple factories to load 3D models in a few lines of code
- Advanced factories (`Spawners`) to load complex models and fine tune memory usage
  - It is possible to share a same object hundreds of times to lower memory usage (rendering 100 objects is the same price in memory as rendering 1)
- Game objects that can be easily transformed with regular mathematical transformations (translation, rotation, shrinking/growing)
- Simple, general purpose shaders are provided (phong lighting)
- A multi-platform input-system to be able to move objects around
- An FPS-camera controller so that players can fly, run in generated 3D worlds
- A simple App class to be able to start an app in a few lines of code
- This class is event-driven
