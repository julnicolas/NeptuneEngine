# Programming Guidelines


## Engine Structure

Neptune is made up of several basic independant modules (graphics, AI,
sound...) called "Core Modules". 

Besides, Neptune is also composed of modules which combine core modules to implement
high level features. They are called "High Level Modules".

Finally, Neptune dispose of a set of interfaces which represent standard behaviors
useful througout the engine (from any core module to any high level one). We will
refer to the elements of the set as "Standard Interfaces". 


## Rules To Add A New Class

- Any class must inherit at most from one and only one other class. 
- A class can inherit from several standard interfaces.
- Any other engine-class dependency should be achieved through composition.


## Platform Creation

Every new physical platform must have a Core and HighLevel directories.

New platforms must be created in `Engine/` (please read the next sections to learn more about supported platforms).

The signification of each directory name is as follows:
- Core: Contains core modules only. A High level module is made up of
core modules.

- High Level: Any module that relies on core modules.

The `Win32` historically has a Project directory (which is going to disappear):.
It contains any file that permits the users to build the engine on 
their machine.

## Supported Platforms

Here is the list of supported platforms with their specifics:
- `Win32` - code necessary to compile the engine for Windows PCs:
    - intel x86-64 architecture
    - Uses openGL and SDL2
- `Linux` - code necessary to compile the engine for Linux computers:
    - intel x86-64 architecture
    - Uses openGL and SDL2
- `MultiPlatform` - logical platform:
    - code is guaranted to be functionnal for *all* supported platforms
- `SharedSpecifics` - logical platform: <-- `Some refactoring is needed here, this is not logical`
    - implementations shared by several platforms
    - code present here do not target all platforms