# Porting to Linux

## List Of Dependencies
### .so files to compile for linux based on Windows dependencies

Note - glew is Windows specific so shouldn't be imported

```
Win32
├── Bin
│   ├── X64
│   │   ├── assimp.dll
│   │   ├── assimp.lib
│   │   ├── Debug
│   │   │   └── libktx.gl.lib
│   │   ├── glew32.dll
│   │   ├── glew32.lib
│   │   ├── Release
│   │   │   └── libktx.gl.lib
│   │   ├── SDL2.dll
│   │   ├── SDL2.lib
│   │   ├── SDL2main.lib
│   │   └── SDL2test.lib
│   └── X86
│       ├── assimp.dll
│       ├── assimp.lib
│       ├── Debug
│       │   └── libktx.gl.lib
│       ├── glew32.dll
│       ├── glew32.lib
│       ├── Release
│       │   └── libktx.gl.lib
│       ├── SDL2.dll
│       ├── SDL2.lib
│       ├── SDL2main.lib
│       └── SDL2test.lib
```

## List of classes to implement
This is the listing of all the files to be implemented for Linux.
It is based on listing all Windows-specific files.

```
Linux
├── Core
│   ├── Graphics
│   │   └── DisplayDeviceInterface.cpp
│   ├── Input
│   │   ├── EventSystemInterface.cpp
│   │   ├── InputEvent.h
│   │   ├── InputProducerFactory.cpp
│   │   ├── PlatformSpecificInputTypes.h
│   │   ├── SDLInputProducer.cpp
│   │   └── SDLInputProducer.h
│   └── Math
│       ├── Geometry
│       │   └── TrigonometryImplementation.h
│       └── Vectors
│           ├── Mat2Implementation.h
│           ├── Mat2x3Implementation.h
│           ├── Mat2x4Implementation.h
│           ├── Mat3Implementation.h
│           ├── Mat3x2Implementation.h
│           ├── Mat3x4Implementation.h
│           ├── Mat4Implementation.h
│           ├── Mat4x2Implementation.h
│           ├── Mat4x3Implementation.h
│           ├── MatrixTransformImplementation.h
│           ├── Vec2Implementation.h
│           ├── Vec3Implementation.h
│           └── Vec4Implementation.h
├── HighLevel
│   ├── Application
│   │   └── Controller
│   │       └── WindowController.cpp
│   └── Camera
│       └── Controller
│           ├── FPSController.cpp
│           └── TempFPSCameraController.cpp
```