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

## Problems encountered
This project for 42 is a first introduction to raytracing. We are not meant to use libraries like OpenGL or Vulkan, but to create our own raytracer. So also we will not implement GPU optimisations. Our raytracer will be slow which will be a challenge for itself.

### optimisations


## A ray 
A ray is a line that starts at a point and goes in a particular direction. The equation for a line in 2D or 3D is essencially the same.  
considering a Point A and a Direction b the line is a typical stepping function.
$$  
P(t) = A + tb  
$$    
Where:
t is a scalar param tipically representing time.  So I have on my line a point A and a direction b, I can move along the line by changing the value of t and get P.
If A and P are points in 3D space, then I have automatially a 3D line which can also be expressed like this:  
$$  
(x,y,z)=(x0,y0,z0)+t(a,b,c)  
$$  

## The Viewport
The viewport is a virtual rectangle in the 3D world that contains the grid of image pixel locations. If pixels are spaced the same distance horizontally as they are vertically, the viewport that bounds them will have the same aspect ratio as the rendered image. The distance between two adjacent pixels is called the pixel spacing, and square pixels is the standard. 
We'll initially set the distance between the viewport and the camera center point to be one unit. This distance is often referred to as the focal length.  
While our 3D space has the conventions above, this conflicts with our image coordinates, where we want to have the zeroth pixel in the top-left and work our way down to the last pixel at the bottom right. This means that our image coordinate Y-axis is inverted: Y increases going down the image. 
 We'll also have the y-axis go up, the x-axis to the right, and the negative z-axis pointing in the viewing direction. (This is commonly referred to as right-handed coordinates.)

## right hand vs left hand coordinate system
In a right-handed coordinate system, the x-axis points to the right, the y-axis points up, and the z-axis points out of the screen towards the viewer. The book and tutorials raytracing in one weekend and the next week use a right-handed coordinate system while the raytracer challenge uses a left-handed coordinate system. As does pixar for instance. There is no right or wrong. it is just a convention. We will use the right-handed coordinate system in this project. 

On your right hand, the thumb is the x-axis, the index is the y-axis and the middle finger is the z-axis pointing to you. This is the right-handed coordinate system.

## what are radians
One radian is the angle subtended at the center of a circle by an arc that is equal in length to the radius of the circle. So a full circle is $2π$ radians.
$$
radians(deg) = deg/180 * \pi
$$

Since our input is in degrees and the `cos` and `sin` functions in C use radians, we need to convert the degrees to radians.
$$
degrees(radians) = radians * 180 / \pi
$$

## viewports are virtual
Really the size of the viewport is arbitrary. It will have the same aspect ratio of the image we want to render.  
the idea is that we know the amount of pixel to render which is in the image width and height. Taking a camera "eye" at a certain point and looking at a certain direction, we can imagine the viewport as a window in front of the camera. The center of the viewport is a point and viewport is orthogonal to the ray from the camera center to the middle of the viewport. 
There are two ways to create the viewport:
- With the field of view, the angle from the cam to the viewport width (horizontal fov) or height (vertical fov). having one of this angles and the aspect ratio we can calculate the other. 
- With the width and height of the viewport if we are given a depth of view, the distance beteween the camera eye and the viewport. 

The viewport size doesnt matter unless you want to use the focal length to calculate the depth of field and what will be in focus.  The viewport can be very close to the eyw and so very small, still we will send a ray per pixel out and going through the viewport. 


## Which side of the plane is the normal
In the literature there are two versions. When we calculate the normal for a sphere (and each geometrical obj will have its own formula for the normal...) we have two possible interpretations.  
In this book we consider the normal to always pointing out of the sphere. The formula is a normalized vector: The point of intersection - the center of the sphere divided by the radius.
If my ray is in opposite direction of the normal, then I am outside the sphere. If the ray is in the same direction as the normal, then I am inside the sphere. 
The boolean to know if I am inside or outside the sphere is the dot product of the ray and the normal. If the dot product is positive, then I am outside the sphere.  
I will use this information to calculate the color of the pixel and reflections. 

## Lambertian Reflection Model

- Diffuse reflection: Light is scattered uniformly in all directions.
- No specular component: There's no shiny or mirror-like reflection.
- Idealized surface: Assumes a perfectly rough surface that scatters light equally in all directions.
- Mathematical representation: The reflected light intensity is proportional to the cosine of the angle between the incoming light direction and the surface normal.

# Phong
## Phong Reflection Model
Bui Tuong Phong was a Vietnamese-born computer graphics researcher. He developed the Phong reflection model while working on his Ph.D. at the University of Utah.  
Phong specular reflection is a component of the Phong reflection model, which is used in computer graphics to simulate the way light interacts with surfaces. The model was introduced by Bui Tuong Phong in his 1975 paper "Illumination for Computer Generated Pictures."  

It accounts for both diffuse and specular components simulating a shiny or mirror-like reflection, where light is reflected in a specific direction. It has a roughness parameter to control the degree of roughness of the surface. A smaller roughness value results in a shinier surface. The reflected light intensity is a combination of a diffuse component (similar to Lambertian) and a specular component based on the angle between the reflected light direction and the viewer's direction.  

So the components are:  
- Ambient Reflection: Simulates the constant light that is present in the environment.  
- Diffuse Reflection: Simulates the light scattered in many directions from a rough surface.  
- Specular Reflection: Simulates the light that reflects in a specific direction from a shiny surface, creating highlights. It depends on the viewer's position, the light source position, and the surface normal. The intensity of the specular reflection is calculated using the angle between the viewer direction and the reflection direction of the light.  



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

I used the following resources:  
- [Raytracing in one weekend](https://raytracing.github.io/books/RayTracingInOneWeekend.html)  
- [Raytracing the next week](https://raytracing.github.io/books/RayTracingTheNextWeek.html)  
- [Raytracing the rest of your life](https://raytracing.github.io/books/RayTracingTheRestOfYourLife.html)  
- https://www.desmos.com/3d
- stb_image.h, a header-only image library available on GitHub at https://github.com/nothings/stb.
- [https://gabrielgambetta.com/computer-graphics-from-scratch/](https://gabrielgambetta.com/computer-graphics-from-scratch/)  
- A raytracer on the back of a business card. [https://fabiensanglard.net/rayTracing_back_of_business_card/](https://fabiensanglard.net/rayTracing_back_of_business_card/)

Here are a few really good resources by [Fabien Sanglard](https://fabiensanglard.net/about/index.html):
- scratchapixel.com : Great raytracer lessons written by professionals that have worked on Toy Story, Avatar, Lord of the Rings, Harry Potter, Pirates of the Caribbean and many other movies.  
- An Introduction to Ray Tracing : An old book but a Classic.  
- Physically Based Rendering : Heavy on maths but really good and well explained.  
- http://cosinekitty.com/raytrace/raytrace_us.pdf  

References:
https://graphicscodex.com/app/app.html  

Nice visualizing website:  
https://www.desmos.com/calculator  


Bump map  
https://assetsvfx.gumroad.com/l/uHyhPT?layout=profile

Get the outlines black and white - edge detection  
https://en.wikipedia.org/wiki/Sobel_operator  



## GBD - GNU Debugger
debug with 
```bash
gdb -tui ./miniRT
set args scene/earth.rt 
```
if the interface is scrambled ctrl-l to refresh the screen.  
```bash
b main
r
p *variable
n 
//when segfault where command shows the trace
where 
``` 

## trivia  
Phong and Crow (1975) first introduced the idea of interpolating per-vertex shading normals to give the appearance of smooth surfaces from polygonal meshes.   

## courses  
http://graphics.cs.cmu.edu/courses/15-463/  
The following textbooks can also be useful references in general. All of them are available online from the CMU library:

    Computer Vision: Algorithms and Applications, by Richard Szeliski.
    Computational Imaging Book, by Ayush Bansai, Achuta Kadambi, and Ramesh Raskar.
    Multiple View Geometry in Computer Vision, by Richard Hartley and Andrew Zisserman.
    Computer Vision: A Modern Approach, by David Forsyth and Jean Ponce.
    Foundations of 3D Computer Graphics, by Steven Gortler.
    Digital Image Processing, by Rafael Gonzalez and Richard Woods.
    Photography, by Barbara London and John Upton.

https://www.realtimerendering.com/raytracinggems/  
https://github.com/Apress/ray-tracing-gems  

https://github.com/Apress/Ray-Tracing-Gems-II  
https://link.springer.com/book/10.1007/978-1-4842-7185-8  
https://youtu.be/gBPNO6ruevk?si=Qtmk42NVdP9aJgRN  

https://alain.xyz/blog/ray-tracing-denoising  
