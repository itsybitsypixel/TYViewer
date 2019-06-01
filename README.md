# TYViewer
A model viewer for Ty the Tasmanian Tiger.

![preview](preview.png)

# Controls
**WASD** to move camera\
**LShift** speed up movement\
**LCTRL** slow down movement

# Using
First you'll need to extract the files needed from the game. To do this, use either my [TYExtractor](https://github.com/Pixeln/TYExtractor/releases) or [rkv-extract](https://github.com/Dnawrkshp/ty-1-tools/tree/master/rkv-extract) by [Dnawrkshp](https://github.com/Dnawrkshp).\
After that you can locate a model file based on its **.mdl** extension and simply drag it onto the **TYViewer** executable.

# Requirements (for using)
* OpenGL 3.3 compatible GPU

# Requirements (for building source)
* [GLAD](https://glad.dav1d.de/)
* [GLFW](https://www.glfw.org/)
* [GLM](https://glm.g-truc.net/0.9.9/index.html)
* [SOIL 2](https://bitbucket.org/SpartanJ/soil2)

# TODO
There is still a lot of stuff that I need to do.
* Implement materials.
* Implement animations.
* Allow for manipulating meshes.
* Allow loading in multiple models.
* Allow loading in levels with props.
* Support for Ty the Tasmanian Tiger 2 models.
* Documentation about **.mdl**, **global.mad**, and **global.model** files.
