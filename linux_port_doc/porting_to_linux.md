# Porting to Linux

## List Of Dependencies
### .so files to compile for linux based on Windows dependencies

Note - glew is Windows specific so shouldn't be imported

Win32
│       ├── Bin
│       │   ├── X64
│       │   │   ├── assimp.dll
│       │   │   ├── assimp.lib
│       │   │   ├── Debug
│       │   │   │   └── libktx.gl.lib
│       │   │   ├── glew32.dll
│       │   │   ├── glew32.lib
│       │   │   ├── Release
│       │   │   │   └── libktx.gl.lib
│       │   │   ├── SDL2.dll
│       │   │   ├── SDL2.lib
│       │   │   ├── SDL2main.lib
│       │   │   └── SDL2test.lib
│       │   └── X86
│       │       ├── assimp.dll
│       │       ├── assimp.lib
│       │       ├── Debug
│       │       │   └── libktx.gl.lib
│       │       ├── glew32.dll
│       │       ├── glew32.lib
│       │       ├── Release
│       │       │   └── libktx.gl.lib
│       │       ├── SDL2.dll
│       │       ├── SDL2.lib
│       │       ├── SDL2main.lib
│       │       └── SDL2test.lib

## List of classes to implement

