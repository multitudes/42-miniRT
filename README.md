[![42](https://img.shields.io/badge/-Berlin-blue.svg?logo=data:image/svg%2bxml;base64,PD94bWwgdmVyc2lvbj0iMS4wIiBlbmNvZGluZz0idXRmLTgiPz4NCjwhLS0gR2VuZXJhdG9yOiBBZG9iZSBJbGx1c3RyYXRvciAxOC4xLjAsIFNWRyBFeHBvcnQgUGx1Zy1JbiAuIFNWRyBWZXJzaW9uOiA2LjAwIEJ1aWxkIDApICAtLT4NCjxzdmcgdmVyc2lvbj0iMS4xIg0KCSBpZD0iQ2FscXVlXzEiIHNvZGlwb2RpOmRvY25hbWU9IjQyX2xvZ28uc3ZnIiBpbmtzY2FwZTp2ZXJzaW9uPSIwLjQ4LjIgcjk4MTkiIHhtbG5zOnJkZj0iaHR0cDovL3d3dy53My5vcmcvMTk5OS8wMi8yMi1yZGYtc3ludGF4LW5zIyIgeG1sbnM6c3ZnPSJodHRwOi8vd3d3LnczLm9yZy8yMDAwL3N2ZyIgeG1sbnM6c29kaXBvZGk9Imh0dHA6Ly9zb2RpcG9kaS5zb3VyY2Vmb3JnZS5uZXQvRFREL3NvZGlwb2RpLTAuZHRkIiB4bWxuczpkYz0iaHR0cDovL3B1cmwub3JnL2RjL2VsZW1lbnRzLzEuMS8iIHhtbG5zOmNjPSJodHRwOi8vY3JlYXRpdmVjb21tb25zLm9yZy9ucyMiIHhtbG5zOmlua3NjYXBlPSJodHRwOi8vd3d3Lmlua3NjYXBlLm9yZy9uYW1lc3BhY2VzL2lua3NjYXBlIg0KCSB4bWxucz0iaHR0cDovL3d3dy53My5vcmcvMjAwMC9zdmciIHhtbG5zOnhsaW5rPSJodHRwOi8vd3d3LnczLm9yZy8xOTk5L3hsaW5rIiB4PSIwcHgiIHk9IjBweCIgdmlld0JveD0iMCAtMjAwIDk2MCA5NjAiDQoJIGVuYWJsZS1iYWNrZ3JvdW5kPSJuZXcgMCAtMjAwIDk2MCA5NjAiIHhtbDpzcGFjZT0icHJlc2VydmUiPg0KPHBvbHlnb24gaWQ9InBvbHlnb241IiBwb2ludHM9IjMyLDQxMi42IDM2Mi4xLDQxMi42IDM2Mi4xLDU3OCA1MjYuOCw1NzggNTI2LjgsMjc5LjEgMTk3LjMsMjc5LjEgNTI2LjgsLTUxLjEgMzYyLjEsLTUxLjEgDQoJMzIsMjc5LjEgIi8+DQo8cG9seWdvbiBpZD0icG9seWdvbjciIHBvaW50cz0iNTk3LjksMTE0LjIgNzYyLjcsLTUxLjEgNTk3LjksLTUxLjEgIi8+DQo8cG9seWdvbiBpZD0icG9seWdvbjkiIHBvaW50cz0iNzYyLjcsMTE0LjIgNTk3LjksMjc5LjEgNTk3LjksNDQzLjkgNzYyLjcsNDQzLjkgNzYyLjcsMjc5LjEgOTI4LDExNC4yIDkyOCwtNTEuMSA3NjIuNywtNTEuMSAiLz4NCjxwb2x5Z29uIGlkPSJwb2x5Z29uMTEiIHBvaW50cz0iOTI4LDI3OS4xIDc2Mi43LDQ0My45IDkyOCw0NDMuOSAiLz4NCjwvc3ZnPg0K)](https://42berlin.de) [![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT) ![Version](https://img.shields.io/badge/version-0.1.1-blue) 

# 42-miniRT
> May your pixels never be little squares. - GH

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

## The Math behind our mini raytracer

(These are my notes from online resources and what I learned in the process.)
Our implementation is sometimes called Montecarlo Raytracing because it makes heavily use of probability functions to 
achieve better photorealism in the scenes. Read more about the probability density functions at the heart of our miniRT [here](pdf.md).  

At its core, a ray tracer sends rays through pixels and computes the color seen in the direction of those rays. 

The involved steps are:

- Calculate the ray from the “eye” through the pixel,
- Determine which objects the ray intersects, and
- Compute a color for the closest intersection point.

## The aspect ratio
A 16∶9 aspect ratio means that the ratio of image width to image height is 16∶9.  
For a practical example, an image 800 pixels wide by 400 pixels high has a 2∶1 aspect ratio. 

## The Vec3, Color and Point classes
For simplicity we will use the same struct (in C) for all three. The Vec3 struct will be used for vectors, colors, and points using a typedef and a union because when using a color having access to r g b is more intuitive than x y z. 
 
## Using unions in C
A union is a special data type available in C that allows to store different data types in the same memory location. You can define a union with many members, but only one member can contain a value at any given time. Unions provide an efficient way of using the same memory location for multiple-purpose.
In my case I have a vector in 3d with x y and z components. Also I have a color type with 3 components too. I can use alias to use the same struct for both. Also I discovered that I can use a union to have access to rgb and xyz at the same time.  In C++ I would use encapsulation and private/public members with getters and setters but in C I dont have this level of abstraction.
```c
typedef struct s_vec3 {
    union {
        struct {
            double x, y, z;
        };
        struct {
            double r, g, b;
        };
    };
} t_vec3, t_color, t_point3;
```

## The Ray
The ray is essentially a function that takes a param a point in 3D space, and moves this point along the direction vector of the ray. The amount of movement is determined by the time parameter t. Therefore we have a function P(t) = A + tb where A is the origin of the ray and b is the direction of the ray.
This function gives us a point in 3D space for each value of t.

```c
t_point3	point_at(const t_ray *ray, double t)
{
	t_point3	result;
	t_vec3 		scaled_dir;

	scaled_dir = vec3multscalar(ray->dir, t);
	result = vec3add(ray->orig, scaled_dir);

	return (result);
}
```

As part of the camera we have function ray_color that uses a lerp function to blend the colors of the sky and the ground effectively creating a soft gradient.  A lerp is  

$$  
blendedValue = (1 - a) \cdot startValue + a \cdot endValue
$$

with `a` going from zero to one. When we normalize the vector we get a value between -1 and 1. We can then scale it to 0 and 1. 
```c
t_color ray_color(const t_ray *r)
	...
	t_vec3 unit_direction = unit_vector(r->dir);
	double a = 0.5 * (unit_direction.y + 1.0);
	t_color start = vec3multscalar(color(1.0, 1.0, 1.0), 1.0 - a);
	t_color end = vec3multscalar(color(0.5, 0.7, 1.0), a);
	return vec3add(start, end);
```

## The Viewport
The viewport is a virtual rectangle in the 3D world that contains the grid of image pixel locations. If pixels are spaced the same distance horizontally as they are vertically, the viewport that bounds them will have the same aspect ratio as the rendered image. The distance between two adjacent pixels is called the pixel spacing, and square pixels is the standard.  
We'll initially set the distance between the viewport and the camera center point to be one unit. This distance is often referred to as the focal length.  

We create our three D space with the following conventions:
- The x-axis points to the right.
- The y-axis points up.
- The z-axis points out of the screen, toward the viewer.

Therefore using what is commonly defined as right hand coordinates, the negative z-axis points in the scene through the viewport. 

While our 3D space has the conventions above, this conflicts with our image coordinates which are the ones used in the rendering to a file or to a screen, where we want to have the zeroth pixel in the top-left and work our way down to the last pixel at the bottom right. This means that our image coordinate Y-axis is inverted: Y increases going down the image. 

## Ray-Sphere Intersection
(If you see this on the github pages unfortunately Jekill doesnt render the math equations so I will have to debug them out later)
The equation for a sphere of radius r that is centered at the origin is an important mathematical equation: 

$$
x^2 + y^2 + z^2 = r^2
$$

If a given point (x,y,z) is inside the sphere, then $x^2 + y^2 + z^2 < r^2$ , and if a given point (x,y,z) is outside the sphere, then $x^2 + y^2 + z^2 > r^2$.

If we want to allow the sphere center to be at an arbitrary point (Cx,Cy,Cz) in 3D, then the equation becomes:  

$$
(Cx−x)^2 + (Cy−y)^2 + (Cz−z)^2 = r^2
$$

This formula looks very similar to the definition of a dot product of a vector from point P = (x,y,z) to a center C = (Cx,Cy,Cz) which is (C-P).

The dot product of this vector with itself is:  

$$
(C−P)⋅(C−P)=(Cx−x)^2+(Cy−y)^2+(Cz−z)^2 = r ^ 2
$$ 

We can read this as “any point P that satisfies this equation is on the sphere”.  

Remember that on our ray P is a function of t, so we can substitute P = Q + td into the equation and solve for t. (Q is the origin of the ray, and d is the direction of the ray.)

The only unknown is t, and we have a $t^2$, which means that this equation is quadratic. 
You can solve for a quadratic equation $ax^2+bx+c=0$ by using the quadratic formula:

$$
x = \frac{-b \pm \sqrt{b^2 - 4ac}}{2a}
$$

So solving for t in the ray-sphere intersection equation gives us these values for a, b, and c:

$$
a=d⋅d
b=−2d⋅(C−Q)
c=(C−Q)⋅(C−Q)−r2
$$

where Q is the origin of the ray, d is the direction of the ray, C is the center of the sphere, and r is the radius of the sphere.

Really we just need to know if the square root also called the discriminant is real and positive. If it is, then the ray intersects the sphere at two points. If it is zero, then the ray is tangent to the sphere. If it is negative, then the ray misses the sphere entirely.

$$
\sqrt{b^2 - 4ac}
$$

In code:
```c
bool hit_sphere(const t_sphere *s, const t_ray *r) 
{
    t_vec3 cq = vec3substr(&(s->center), &(r->orig));
    double a = dot(&(r->dir), &(r->dir));
    double b = -2.0 * dot(&(r->dir), &cq);
    double c = dot(&cq, &cq) - s->radius * s->radius;
    double discriminant = b*b - 4*a*c;
    return (discriminant >= 0);
}
```

# The Normal Vector and shading
This is a vector that is perpendicular to the surface at the point of intersection.  
Normalizing It is an expensive operation involving taking the square root of the sum of the squares of the components of the vector. Still it needs to be done so all normal vectors will be of unit length.  

Ex For a sphere, the outward normal is in the direction of the hit point minus the center:  
A common trick used for visualizing normals (because it’s easy and somewhat intuitive to assume n is a unit length vector — so each component is between −1 and 1) is to map each component to the interval from 0 to 1, and then map (x,y,z) to (red,green,blue). 

## Which side of the sphere are we on?
We need to choose to determine the side of the surface at the time of geometry intersection or at the time of coloring.  
For objects that have an inside and an outside, like glass balls, we need to know if the ray is inside or outside the sphere.  
It can be determined doing the dot product of the ray direction and the outward normal. If the dot product is positive, the ray is inside the sphere. If it is negative, the ray is outside the sphere.   

```c
bool front_face;
if (dot(ray_direction, outward_normal) > 0.0) {
    // ray is inside the sphere
    normal = -outward_normal;
    front_face = false;
} else {
    // ray is outside the sphere
    normal = outward_normal;
    front_face = true;
}
```

## The hittable property

The book makes an “abstract class” for anything a ray might hit, and make both a sphere and a list of spheres just something that can be hit. How to do this in C?

##  An array of shapes in C? How!?
Thanks to this course I discovered that you can have polymorphic behavior in C. 
It is a bit weird at first but totally doable.  

I have a struct, `t_hittable` that has a function pointer to a hit function.  

To create an array of different shapes (like spheres and cubes) that all implement the [`hittable`]interface or behaviour, I define a struct `t_hittable` that contains a function pointer for the `hit` function:

```c
typedef struct s_hittable {
    bool (*hit)(const void* self, const t_ray* r, double t_min, double t_max, t_hit_record* rec);
} t_hittable;
```

Then, for each shape (like a sphere or a cube), you create a struct that includes the `t_hittable` struct as its first member:

```c
typedef struct {
    t_hittable base;
    t_point3 center;
    double radius;
} sphere;

typedef struct {
    t_hittable base;
    t_point3 min;
    t_point3 max;
} cube;
```
**This is very important: the `hittable` struct must be the first member of the shape structs.**  
This is because the C standard guarantees that the address of the first member of a struct is the same as the address of the struct itself. This allows you to cast a pointer to a `sphere` or `cube` to a `hittable` pointer and back without any issues.

You can then define the `hit` functions for each shape:

```c
bool hit_sphere(const void* self, const t_ray* r, double t_min, double t_max, t_hit_record* rec) {
    // implementation...
}

bool hit_cube(const void* self, const t_ray* r, double t_min, double t_max, t_hit_record* rec) {
    // implementation...
}
```

The functions all take a `const void* self` parameter, which is a pointer to the shape struct. This allows you to cast the pointer to the appropriate shape struct inside the function.

When you create a new shape, you set the `hit` function pointer in the `hittable` struct to the appropriate function:


```c
t_sphere new_sphere(t_point3 center, double radius) {
	sphere s;
	s.base.hit = hit_sphere;
	s.center = center;
	s.radius = radius;
	return s;
}
```
Then in the main function, you can create a list of `t_hittable` pointers and add your shapes to it:

```c
t_hittable *list[2];
t_sphere s1 = sphere(vec3(0, 0, -1), 0.5);
t_sphere s2 = sphere(vec3(0, -100.5, -1), 100);
list[0] = (t_hittable*)(&s1);
list[1] = (t_hittable*)(&s2);
const t_hittablelist world = hittablelist(list, 2);
```

This already works and I get the second sphere creating a nice background.

To check if a ray hits any of the shapes, you can loop over the array and call the `hit` function through the function pointer:

```c
for (int i = 0; i < 10; i++) {
    if (shapes[i]->hit(shapes[i], r, t_min, t_max, &rec)) {
        // hit
    }
}
```
This would be quite cool but turns out that we will use an extra struct to keep track of the world. Since c++ has vectors which are a kind of dynamic array in C, I created a t_hittablelist struct that contains an array of hittable pointers and a size. Knowing the size is important. For now that's it, later I will add some more functions to add elements etc.

```c

typedef struct 	s_hittablelist {
	t_hittable 	**list;
	int 		size;
	bool 		(*hit)(const void* self, const t_ray* r, double ray_tmin, double ray_tmax, t_hit_record* rec);
} 				t_hittablelist;
```

And so we have a hit function on world that will loop through the list of hittables and call their hit functions.

```c
bool hit(const void *self, const t_ray* r, double ray_tmin, double ray_tmax, t_hit_record* rec)
{
	 	t_hittablelist *hittablelist = (t_hittablelist *)self;
		t_hit_record temp_rec;
		bool hit_anything = false;
		double closest_so_far = ray_tmax;
		int i = 0;

		while (i < hittablelist->size)
		{
			if (hittablelist->list[i]->hit(hittablelist->list[i], r, ray_tmin, closest_so_far, &temp_rec))
			{
				hit_anything = true;
				closest_so_far = temp_rec.t;
				*rec = temp_rec;
			}
			i++;
		} 
	return (hit_anything);
}

t_hittablelist hittablelist(t_hittable **list, int size)
{
	t_hittablelist hittablelist;

	hittablelist.list = list;
	hittablelist.size = size;
	hittablelist.hit = hit;
	return (hittablelist);
}
```

## Useful to have - An Interval type

In C++ it is a class, in C it will be a struct. The interval type is useful to keep track of the t_min and t_max values. 
```c
typedef struct {
	double t_min;
	double t_max;
} t_interval;
```

## The Camera
In C we do not have classes of course but we will still refactor the code in its own file with its struct and functions.
The camera is responsible for two important jobs:

    - Construct and dispatch rays into the world.
    - Use the results of these rays to construct the rendered image.

After main creates a camera and sets default values, it will call the render() method.  
The render() method will prepare the camera for rendering and then execute the render loop.

## Passing by value or by reference?
In C++ we have references which are variables that can be passed around without copying. In C we have pointers. But pointers bring a problem. 
If I declare the sum like
```c
t_vec3 add(t_vec3 *a, t_vec3 *b)
{
	t_vec3 sum;
	sum.x = a->x + b->x;
	sum.y = a->y + b->y;
	sum.z = a->z + b->z;
	return (sum);
}
```
Then if using the add functions for two vectors I have to pass pointers then I cannot use the add function with the result of another add function. Like this is working but I have to create a temp extra var c:
```c
t_vec3 a = vec3(1, 2, 3);
t_vec3 b = vec3(4, 5, 6);
t_vec3 c = add(&a, &b);
t_vec3 d = add(&c, &c);
```
This will not work because the add function expects pointers and I cannot in C take the pointer of a rvalue return value . So I will have to use the add function like this:
```c
t_vec3 a = vec3(1, 2, 3);
t_vec3 b = vec3(4, 5, 6);
t_vec3 c = vec3(&add(&a, &b), &add(&a, &b));
```

With small structs there is a very small performance hit when passing by value. So I will pass by value and this allows to chain functions and have better readable code.
```c
t_vec3 add(t_vec3 a, t_vec3 b)
{
	t_vec3 sum;
	sum.x = a.x + b.x;
	sum.y = a.y + b.y;
	sum.z = a.z + b.z;
	return (sum);
}
```
now:
```c
t_vec3 a = vec3(1, 2, 3);	
t_vec3 b = vec3(4, 5, 6);
t_vec3 d = add(add(a, b), add(a, b));
```
It is much nicer.


## Antialiasing
If we look at our zoomed image created until now we see that the edges are jagged like pixels on a staircase. To fix this we will take multiple samples per pixel and average the color. This is called antialiasing. In the book also there is an interesting note about the human eye and how it perceives color.
> With a single ray through the center of each pixel, we are performing what is commonly called point sampling. The problem with point sampling can be illustrated by rendering a small checkerboard far away. If this checkerboard consists of an 8×8 grid of black and white tiles, but only four rays hit it, then all four rays might intersect only white tiles, or only black, or some odd combination. In the real world, when we perceive a checkerboard far away with our eyes, we perceive it as a gray color, instead of sharp points of black and white.

We need to sample the light falling around the pixel, and integrate the changes.

To do so we will create a random number generator to sample the colors around the pixel in a random way. 


## Extras - the compile_commands.json file
If using nvim the LSP will ask for a compile_commands.json file. This file is can be generated by the compiledb tool. 
```
make clean && make > build.log
compiledb -n < build.log      
```

## Diffuse materials

Diffuse objects that don’t emit their own light merely take on the color of their surroundings, but they do modulate that with their own intrinsic color. Light that reflects off a diffuse surface has its direction randomized.  
We will add the ability to generate arbitrary random vectors to our t_vec3 type. This will be useful for generating random directions for diffuse materials. 
In c++ they are both called random but I cannot use the same name in C. I will call them random_vec3 and random_vec3_min_max.
```c
t_vec3 random_vec3() 
{
    return vec3(random_d(), random_d(), random_d());
}

t_vec3 random_vec3_min_max(double min, double max) 
{
    return vec3(random_double(min,max), random_double(min,max), random_double(min,max));
}
```
Then we need to figure out how to manipulate a random vector so that we only get results that are on the surface of a hemisphere, the side where we have our normal.  
The book explains that we can generate a random vector in the unit cube and then check if it is inside the unit sphere. If it is not we discard it. This is called rejection sampling.

## Shadow acne
Do to floating points errors we ignore hits that are very close to the calculated intersection point. Due to these errors we can calculate a hit point that is just a bit below the surface of the object. This is called shadow acne. 
To fix this we will add a small epsilon value to the t_min value in my ray_color function.
```c
if ((world)->hit(world, r, interval(0.001, INFINITY), &rec))
	{
		t_vec3 direction = random_on_hemisphere(rec.normal);
		t_ray scattered = ray(rec.p, direction);
		return vec3multscalar(ray_color(&scattered, depth - 1, world), 0.5);
	}
```

This is why we have intervals :)  

The result is now gray and it takes quite a while to generate due to the recursive method in our ray_color function.  


## Lambertian reflection
A reflected ray is most likely to scatter in a direction near the surface normal.
This makes very much sense but I refer to the book for the explanation!
It is a small change in the ray_color function. 
```c
// from 
	t_vec3 direction = random_on_hemisphere(rec.normal);
// to	
	t_vec3 direction = vec3add(rec.normal, random_unit_vector());
```
It is not terribly visible but the image is a bit more realistic.



## Gamma correction
The book explains that the human eye does not perceive light linearly. The eye is more sensitive to changes in darker colors than in lighter colors. This is why we need to apply gamma correction to our image.  
We need to go from linear space to gamma space, which means taking the inverse of “gamma 2", which means an exponent of 1/gamma, which is just the square-root. 

Not bad. The image is muc brighter now for .5 grey.


## Metal
The material needs to do two things:

	- Produce a scattered ray (or say it absorbed the incident ray).
    - If scattered, say how much the ray should be attenuated.

In C++ it is an abstract class. In C I will use a struct with a function pointer to a hit function. 

```c
typedef struct s_material {
	bool (*scatter)(const void* self, const t_ray* r_in, const t_hit_record* rec, t_vec3* attenuation, t_ray* scattered);
} t_material;
```
We are going to put a pointer to a material in our hit record.  
Also, hit_record needs to be told the material that is assigned to the sphere.  Our sphere struct will have a material pointer.  
 

## Albedo
The term albedo (Latin for “whiteness”) is used to describe the fraction of incident light that is reflected by a surface. It is used to define some form of fractional reflectance. Albedo will vary with material color.  
We will implement Lambertian (diffuse) reflectance first. This is the simplest form of reflection.  

How to implement this in C where in C++ we would have a virtual function?
Objects like my sphere will have pointers to a `t_material` struct and this `t_material` struct is also included in the `t_hit_record`:

- I will define a `material` struct with a function pointer for the `scatter` function. This function determines how materials interact with rays.

- In `hit_record` include a pointer to the `material` of the object hit.

- Objects (e.g., spheres) include a pointer to the `t_material` struct, 

- Implement functions for different materials. These functions should match the signature of the `scatter` function pointer in the `material` struct.

- When creating objects, assign a material to them by setting the material pointer to point to an instance of a `material` struct configured with the appropriate scatter function.

- When a ray hits an object, the `t_hit_record` will be populated with information about the hit, including the material of the object. The ray tracing logic can then use the `scatter` function through the `t_hit_record`'s material pointer to determine the color and direction of the scattered ray.

But since I have different materials with different properties I will create a struct for each material with their unique properties. Ex the lambertian material has albedo. The Base in the struct is the material pointer and then I have the specific properties. I can pass this struct to the scatter function since it expects a material pointer and I have this in the base struct. 

ex:
```c
// Example of creating a lambertian material
t_lambertian lambertian_material;

// Function to initialize a Lambertian material
void lambertian_init(t_lambertian *lambertian_material, t_color albedo) 
{
    lambertian_material->base.scatter = lambertian_scatter; // Assign the scatter function
    lambertian_material->albedo = albedo; // Set the albedo
}

bool lambertian_scatter(void* self, const t_ray *r_in, const t_hit_record *rec, t_color *attenuation, t_ray *scattered) 
{
	(void)r_in;
	t_lambertian *lamb = (t_lambertian *)self;
	t_vec3 scatter_direction = vec3add(rec->normal, random_unit_vector());
    *scattered = ray(rec->p, scatter_direction);
    *attenuation = lamb->albedo;
        return true;
    return true; 
}
```

Lambertian surfaces scatter all incoming light uniformly in all directions, but the amount of light scattered is proportional to the surface's albedo.

In the `scatter` function of the `lambertian` class, the `albedo` is used to set the `attenuation` of the scattered ray. The `attenuation` represents how much the ray's intensity is reduced upon scattering. For a Lambertian material, this attenuation is simply the albedo of the material, indicating that the color of the scattered light is influenced by the color of the material itself.

Here's a breakdown of the process:  

- Scatter Direction:  new direction for the scattered ray is calculated by adding a random unit vector to the hit normal. This simulates diffuse reflection, where light is scattered in many directions.
- Attenuation: The `attenuation` parameter is set to the material's `albedo`. This means the scattered light's color is influenced by the material's color, simulating how real-world surfaces absorb some wavelengths of light more than others, thereby coloring the light that is reflected.
- Scattered Ray: The `scattered` ray is created with its origin at the point of intersection (`rec.p`) and its direction set to the calculated scatter direction. This ray represents the path of the light after it has interacted with the material.

## Metals
For polished metals the ray won’t be randomly scattered. 
Follow the "Raytracing in a weekend" tutorial for the math explanation but for metals and polished surface we need to calculate the reflection of the ray.
The reflection of a ray is calculated by reflecting the ray direction across the surface normal. This is done by subtracting the ray direction from the normal twice. 
```c
inline t_vec3	vec3reflect(const t_vec3 v, const t_vec3 n) 
{
    return vec3substr(v, vec3multscalar(n, dot(v, n) * 2));
}
```


## Fuzziness
We can also randomize the reflected direction by using a small sphere and choosing a new endpoint for the ray. We'll use a random point from the surface of a sphere centered on the original endpoint, scaled by the fuzz factor. The bigger the fuzz sphere, the fuzzier the reflections will be. This suggests adding a fuzziness parameter that is just the radius of the sphere (so zero is no perturbation). Also, we need to normalize the reflected ray. 

## Positionable Camera
### field of view fov
It is typically expressed as an angle (in degrees or radians) and determines how wide or narrow the view captured by the camera is. A larger FOV allows the camera to capture a wider area of the scene, making objects appear smaller and further apart.  In the book we will use vertical field of view by convension since the horizontal fov will be determined by the aspect ratio of the image.
Now I can express the viewport in function of the vertical field of view and the focal_length.
```c
double theta = degrees_to_radians(c.vfov);
double h = tan(theta/2);
double viewport_height = 2 * h * focal_length;
double viewport_width = viewport_height * ((double)c.image_width/c.image_height);
```

## Positionable Camera
We can rotate the camera around its normal axis. We need a way to specify the up. and the look from and look at. 
The tutorial uses the common convention of naming this the “view up” (vup) vector.
We will make the viewport height dependent from the vertical field of view and the aspect ratio. 
```c
cam.vfov = 90;
cam.lookfrom = point3(-2,2,1);   // Point camera is looking from
cam.lookat   = point3(0,0,-1);  // Point camera is looking at
cam.vup      = vec3(0,1,0);     // Camera-relative "up" direction

double theta = degrees_to_radians(cam.vfov);
double h = tan(theta/2);
double viewport_height = 2 * h * focal_length;
double viewport_width = viewport_height * ((double)cam.image_width/cam.image_height);
...
```


## Links

The Graphical library allowed for this project are either the Minilibx or the MLX42 Codam. We used the second one:  
- [https://github.com/codam-coding-college/MLX42](https://github.com/codam-coding-college/MLX42)    

I used the following resources:  
- [Raytracing in one weekend](https://raytracing.github.io/books/RayTracingInOneWeekend.html)  
- [Raytracing the next week](https://raytracing.github.io/books/RayTracingTheNextWeek.html)  
- [Raytracing the rest of your life](https://raytracing.github.io/books/RayTracingTheRestOfYourLife.html)  
- [https://www.desmos.com/3d](https://www.desmos.com/3d)
- stb_image.h, a header-only image library available on GitHub at https://github.com/nothings/stb.
- [https://gabrielgambetta.com/computer-graphics-from-scratch/](https://gabrielgambetta.com/computer-graphics-from-scratch/)  
- A raytracer on the back of a business card. [https://fabiensanglard.net/rayTracing_back_of_business_card/](https://fabiensanglard.net/rayTracing_back_of_business_card/)

Here are a few more really good links by [Fabien Sanglard](https://fabiensanglard.net/about/index.html):
- scratchapixel.com : Great raytracer lessons written by professionals that have worked on Toy Story, Avatar, Lord of the Rings, Harry Potter, Pirates of the Caribbean and many other movies.  
- An Introduction to Ray Tracing : An old book but a Classic.  
- Physically Based Rendering : Heavy on maths but really good and well explained.  
- http://cosinekitty.com/raytrace/raytrace_us.pdf
  
More References:
- [https://graphicscodex.com/app/app.html](https://graphicscodex.com/app/app.html)  
- Bump map  
https://assetsvfx.gumroad.com/l/uHyhPT?layout=profile  
- Get the outlines black and white - edge detection  
https://en.wikipedia.org/wiki/Sobel_operator  
- a great youtube series: [https://www.youtube.com/playlist?list=PLlrATfBNZ98edc5GshdBtREv5asFW3yXl](https://www.youtube.com/playlist?list=PLlrATfBNZ98edc5GshdBtREv5asFW3yXl)  
- a guide to git commits: [https://www.conventionalcommits.org/en/v1.0.0](https://www.conventionalcommits.org/en/v1.0.0)  
- cross product: [https://en.wikipedia.org/wiki/Cross_product#/media/File:Cross_product_mnemonic.svg](https://en.wikipedia.org/wiki/Cross_product#/media/File:Cross_product_mnemonic.svg)  
- and also [https://www.mathsisfun.com/algebra/vectors-cross-product.html](https://www.mathsisfun.com/algebra/vectors-cross-product.html)  
- dot product: [https://www.mathsisfun.com/algebra/vectors-dot-product.html](https://www.mathsisfun.com/algebra/vectors-dot-product.html)
- I looked into the Sobel Operator but I did not implement it in the end: [https://en.wikipedia.org/wiki/Sobel_operator](https://en.wikipedia.org/wiki/Sobel_operator)


## Some online courses  
http://graphics.cs.cmu.edu/courses/15-463/  

The following textbooks are also useful references in general. 
- Computer Vision: Algorithms and Applications, by Richard Szeliski.  
- Computational Imaging Book, by Ayush Bansai, Achuta Kadambi, and Ramesh Raskar.  
- Multiple View Geometry in Computer Vision, by Richard Hartley and Andrew Zisserman.  
- Computer Vision: A Modern Approach, by David Forsyth and Jean Ponce.  
- Foundations of 3D Computer Graphics, by Steven Gortler.  
- Digital Image Processing, by Rafael Gonzalez and Richard Woods.  
- Photography, by Barbara London and John Upton.  
- https://www.realtimerendering.com/raytracinggems/  
- https://github.com/Apress/ray-tracing-gems  
- https://github.com/Apress/Ray-Tracing-Gems-II  
- https://link.springer.com/book/10.1007/978-1-4842-7185-8  
- https://youtu.be/gBPNO6ruevk?si=Qtmk42NVdP9aJgRN  
- https://alain.xyz/blog/ray-tracing-denoising  
- https://planetpixelemporium.com/earth8081.html  
- https://www.solarsystemscope.com/textures/

## Inspiration and credits
Even if my code and approach are completely different, for some scenes I got inspired by some of the following projects:  
- [https://github.com/ricardoreves/42-minirt](https://github.com/ricardoreves/42-minirt), the atom scene, mindblowing!  Nice to see the implementation of the obj files which we did not do.
- [https://github.com/tdameros](https://github.com/tdameros)  For a nice implementation!
- And for denoising tips...  [https://github.com/favreau/awesome-ray-tracing?tab=readme-ov-file#denoising-filtering-and-reconstruction](https://github.com/favreau/awesome-ray-tracing?tab=readme-ov-file#denoising-filtering-and-reconstruction)  


