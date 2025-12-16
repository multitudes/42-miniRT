


## Requirements and Features - A 42 Project

This project is an introduction to the beautiful world of Raytracing.  
The goal is to create a simple raytracer that can render basic objects like spheres, planes, and cylinders.  
*This 42 project is written in C and in accordance to the 42 school norm.*
- All variables have to be declared and defined in separate lines
- Variable declaration has to be on the top and no more than 5
- function parameter cannot be more than 4
- Each function can not have more then 25 lines
- Projects should be created with allowed std functions otherwise it is cheating
- etc [link](https://github.com/42School/norminette/blob/master/pdf/en.norm.pdf).  

Bonuses are encouraged but keeping in mind that to implement a more complex features, it is better to create a more complex project.  
Therefore bonuses can be :
- Add specular reflection to have a full Phong reflection model.
• Color disruption: checkerboard.
• Colored and multi-spot lights.
• One other 2nd degree object: Cone, Hyperboloid, Paraboloid..
• Handle bump map textures.

You can see a preview of the scenes in this [thread](https://github.com/multitudes/42-miniRT/issues/42)

## Allowed functions

Here at 42 we are allowed to use the following functions for this project:

| Function | Description |
| -------- | ----------- |
|open | open a file|
|close | close a file|
|read | read from a file descriptor|
|write | write to a file descriptor|
|printf | formatted output conversion|
|malloc | allocate memory|
|free | free allocated memory|
|perror | print an error message|
|strerror | return string describing error|
|exit | terminate the calling process|
|math library | mathematical functions|
|MinilibX | graphical library|

Also we follow the NORM, a series of rules about linting and formatting of the code. Examples: functions cannot have more than 25 lines; we are not allowed to use "for"-loops, but while loops are allowed; declaring and defining variables in one line is not allowed. etc.

### Requirements
Programming Language: Must be written in C.  
Libraries: Utilizes miniLibX or MLX42, math library functions, and standard C libraries (open, close, read, write, printf, malloc, free, perror, strerror, exit)  
Memory Management: Proper handling and freeing of heap-allocated memory to prevent leaks.  
Makefile: Includes rules for all, clean, fclean, re, and bonus. Compilation with -Wall, -Wextra, and -Werror flags using cc.  
Error Handling: Must exit properly with "Error\n" and an explicit message on encountering any misconfiguration in the scene file.  
File Structure: Accepts scene description files in a specific format, handling elements like Ambient light, Camera, Light, Sphere, Plane, and Cylinder with precise properties.  

We are not meant to use libraries like OpenGL or Vulkan, but to create our own implementation in C. 


### Features

- Scene Description: Reads scenes from a file with .rt extension and renders the image as described.  
- Basic Objects: Supports rendering of planes, spheres, and cylinders.
- Transformations: Objects, lights, and cameras can be translated and rotated (except spheres and lights for rotation).  
- Lighting: Implements ambient and diffuse lighting, including spot brightness and hard shadows.
- Window Management: Fluid window management with seamless transitions and closures via ESC key or window frame red cross.
 
It is a group project. The team is composed of two students.

Multithreading is allowed when doing the bonus part.  You can read more [here](multithreading.md).  
