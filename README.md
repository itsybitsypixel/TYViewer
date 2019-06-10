# TYViewer
A model viewer for Ty the Tasmanian Tiger.

![preview](preview.png)

# Using
Start the program. If a config file does not exist in program directory, one will be created. In the 'config.cfg' file, enter path to 'Data_PC.rkv' (located in the game folder) and which model to load.

A list of model names can be found [here](https://gist.github.com/Pixeln/14d7936cd92c13af976cc48d48741d39).

# Controls
**WASD** to move camera\
**MIDDLE MOUSE** to rotate camera\
**LShift** speed up movement\
**LCTRL** slow down movement


**ALPHA 1** toggle grid\
**ALPHA 2** toggle bounds\
**ALPHA 3** toggle colliders\
**ALPHA 4** toggle bones


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
