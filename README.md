[![42](https://img.shields.io/badge/-Berlin-blue.svg?logo=data:image/svg%2bxml;base64,PD94bWwgdmVyc2lvbj0iMS4wIiBlbmNvZGluZz0idXRmLTgiPz4NCjwhLS0gR2VuZXJhdG9yOiBBZG9iZSBJbGx1c3RyYXRvciAxOC4xLjAsIFNWRyBFeHBvcnQgUGx1Zy1JbiAuIFNWRyBWZXJzaW9uOiA2LjAwIEJ1aWxkIDApICAtLT4NCjxzdmcgdmVyc2lvbj0iMS4xIg0KCSBpZD0iQ2FscXVlXzEiIHNvZGlwb2RpOmRvY25hbWU9IjQyX2xvZ28uc3ZnIiBpbmtzY2FwZTp2ZXJzaW9uPSIwLjQ4LjIgcjk4MTkiIHhtbG5zOnJkZj0iaHR0cDovL3d3dy53My5vcmcvMTk5OS8wMi8yMi1yZGYtc3ludGF4LW5zIyIgeG1sbnM6c3ZnPSJodHRwOi8vd3d3LnczLm9yZy8yMDAwL3N2ZyIgeG1sbnM6c29kaXBvZGk9Imh0dHA6Ly9zb2RpcG9kaS5zb3VyY2Vmb3JnZS5uZXQvRFREL3NvZGlwb2RpLTAuZHRkIiB4bWxuczpkYz0iaHR0cDovL3B1cmwub3JnL2RjL2VsZW1lbnRzLzEuMS8iIHhtbG5zOmNjPSJodHRwOi8vY3JlYXRpdmVjb21tb25zLm9yZy9ucyMiIHhtbG5zOmlua3NjYXBlPSJodHRwOi8vd3d3Lmlua3NjYXBlLm9yZy9uYW1lc3BhY2VzL2lua3NjYXBlIg0KCSB4bWxucz0iaHR0cDovL3d3dy53My5vcmcvMjAwMC9zdmciIHhtbG5zOnhsaW5rPSJodHRwOi8vd3d3LnczLm9yZy8xOTk5L3hsaW5rIiB4PSIwcHgiIHk9IjBweCIgdmlld0JveD0iMCAtMjAwIDk2MCA5NjAiDQoJIGVuYWJsZS1iYWNrZ3JvdW5kPSJuZXcgMCAtMjAwIDk2MCA5NjAiIHhtbDpzcGFjZT0icHJlc2VydmUiPg0KPHBvbHlnb24gaWQ9InBvbHlnb241IiBwb2ludHM9IjMyLDQxMi42IDM2Mi4xLDQxMi42IDM2Mi4xLDU3OCA1MjYuOCw1NzggNTI2LjgsMjc5LjEgMTk3LjMsMjc5LjEgNTI2LjgsLTUxLjEgMzYyLjEsLTUxLjEgDQoJMzIsMjc5LjEgIi8+DQo8cG9seWdvbiBpZD0icG9seWdvbjciIHBvaW50cz0iNTk3LjksMTE0LjIgNzYyLjcsLTUxLjEgNTk3LjksLTUxLjEgIi8+DQo8cG9seWdvbiBpZD0icG9seWdvbjkiIHBvaW50cz0iNzYyLjcsMTE0LjIgNTk3LjksMjc5LjEgNTk3LjksNDQzLjkgNzYyLjcsNDQzLjkgNzYyLjcsMjc5LjEgOTI4LDExNC4yIDkyOCwtNTEuMSA3NjIuNywtNTEuMSAiLz4NCjxwb2x5Z29uIGlkPSJwb2x5Z29uMTEiIHBvaW50cz0iOTI4LDI3OS4xIDc2Mi43LDQ0My45IDkyOCw0NDMuOSAiLz4NCjwvc3ZnPg0K)](https://42berlin.de) [![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT) ![Version](https://img.shields.io/badge/version-0.1.1-blue) 

# 42-miniRT
> May your pixels never be little squares. - GH

This project is an introduction to the beautiful world of Raytracing.  
The goal is to create a simple raytracer that can render basic objects like spheres, planes, and cylinders.  
It is not mean to be a full-featured raytracer, but a simple one that can render a scene from a file.

Bonuses are encouraged but keeping in mind that to implement a more complex features, it is better to create a more complex project.  
Therefore bonuses can be :
- Add specular reflection to have a full Phong reflection model.
• Color disruption: checkerboard.
• Colored and multi-spot lights.
• One other 2nd degree object: Cone, Hyperboloid, Paraboloid..
• Handle bump map textures.


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

### Features
- Scene Description: Reads scenes from a file with .rt extension and renders the image as described.  
- Basic Objects: Supports rendering of planes, spheres, and cylinders.
- Transformations: Objects, lights, and cameras can be translated and rotated (except spheres and lights for rotation).  
- Lighting: Implements ambient and diffuse lighting, including spot brightness and hard shadows.
- Window Management: Fluid window management with seamless transitions and closures via ESC key or window frame red cross.
 
## Team
It is a group project. The team is composed of two students.









<hr>
## Create Random function in C

Since we are not allowed to use the rand() function, we have to create our own random function.
One of the simplest and most commonly used algorithms is the "Linear Congruential Generator (LCG).

1. Define constants `a`, `c`, and `m`.
2. Define a variable `seed` to store the current state.
3. Implement a function to set the seed.
4. Implement a function to generate the next random number using the formula: `seed = (a * seed + c) % m`.

Like:

```c
#include <stdio.h>
#include <time.h>

// Define constants for the LCG
#define A 1664525
#define C 1013904223
#define M 4294967296 // 2^32

// Seed value
unsigned int seed = 1;

// Function to set the seed
void srand_custom(unsigned int new_seed) {
    seed = new_seed;
}

// Function to generate the next random number
unsigned int rand_custom() {
    seed = (A * seed + C) % M;
    return seed;
}

int main() {
    // Set the seed using the current time
    srand_custom((unsigned int)time(NULL));

    // Generate and print 10 random numbers
    for (int i = 0; i < 10; i++) {
        printf("%u\n", rand_custom());
    }

    return 0;
}
```

- The constants `A`, `C`, and `M` are chosen based on common values used in LCGs. These values ensure a good distribution of random numbers.
- The `seed` variable stores the current state of the RNG. It is initialized with a default value but can be set using the `srand_custom` function.
- the `srand_custom` function sets the seed value, allowing you to initialize the RNG with a specific starting point.
- the `rand_custom` function generates the next random number using the LCG formula and updates the seed.

## Phong Reflection
Bui Tuong Phong was a Vietnamese-born computer graphics researcher. He developed the Phong reflection model while working on his Ph.D. at the University of Utah.  
Phong specular reflection is a component of the Phong reflection model, which is used in computer graphics to simulate the way light interacts with surfaces. The model was introduced by Bui Tuong Phong in his 1975 paper "Illumination for Computer Generated Pictures."

Components:  
- Ambient Reflection: Simulates the constant light that is present in the environment.
- Diffuse Reflection: Simulates the light scattered in many directions from a rough surface.
- Specular Reflection: Simulates the light that reflects in a specific direction from a shiny surface, creating highlights. It depends on the viewer's position, the light source position, and the surface normal. The intensity of the specular reflection is calculated using the angle between the viewer direction and the reflection direction of the light.

### Phong Specular Reflection Formula
The specular reflection component is calculated using the following formula:
$$ I_{specular} = k_s \cdot (R \cdot V)^{\alpha} $$
Where:
- $ k_s $: Specular reflection coefficient (material property).
- $ R $: Reflection vector.
- $ V $: View vector (direction from the surface point to the viewer).
- $ \alpha $: Shininess coefficient (controls the size of the highlight).


## Links
- [Raytracing in one weekend](https://raytracing.github.io/books/RayTracingInOneWeekend.html)
- [Raytracing the next week](https://raytracing.github.io/books/RayTracingTheNextWeek.html)
- [Raytracing the rest of your life](https://raytracing.github.io/books/RayTracingTheRestOfYourLife.html)
- stb_image.h, a header-only image library available on GitHub: https://github.com/nothings/stb
- [https://gabrielgambetta.com/computer-graphics-from-scratch/](https://gabrielgambetta.com/computer-graphics-from-scratch/)
- a raytracer on the back of a business card: [https://fabiensanglard.net/rayTracing_back_of_business_card/](https://fabiensanglard.net/rayTracing_back_of_business_card/)
- here are a few really good resources by [Fabien Sanglard](https://fabiensanglard.net/about/index.html):
	- [scratchapixel.com](https://scratchapixel.com): Great raytracer lessons written by professionals that have worked on Toy Story, Avatar, Lord of the Rings, Harry Potter, Pirates of the Caribbean and many 	other movies
	- An Introduction to Ray Tracing: an old book, but a classic
	- Physically Based Rendering: heavy on maths, but really good and well explained

- a great youtube series: [https://www.youtube.com/playlist?list=PLlrATfBNZ98edc5GshdBtREv5asFW3yXl](https://www.youtube.com/playlist?list=PLlrATfBNZ98edc5GshdBtREv5asFW3yXl)
- a guide to git commits: [https://www.conventionalcommits.org/en/v1.0.0](https://www.conventionalcommits.org/en/v1.0.0)
- cross product: [https://en.wikipedia.org/wiki/Cross_product#/media/File:Cross_product_mnemonic.svg](https://en.wikipedia.org/wiki/Cross_product#/media/File:Cross_product_mnemonic.svg)
	- and also [https://www.mathsisfun.com/algebra/vectors-cross-product.html](https://www.mathsisfun.com/algebra/vectors-cross-product.html)
- dot product: [https://www.mathsisfun.com/algebra/vectors-dot-product.html](https://www.mathsisfun.com/algebra/vectors-dot-product.html)



## Links

The Graphical library allowed for this project are either the Minilibx or the MLX42 Codam. We used the second one:  
https://github.com/codam-coding-college/MLX42  