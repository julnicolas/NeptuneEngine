# Install Dependencies

Some dependencies must be installed system wide.

## OpenGL and Mesa (openGL drivers)
On archlinux
``` sh
pacman -S mesa
```

To test the installation is functional:
``` sh
pacman -S mesa-utils
glxgears
```
It should show three rotating gears.