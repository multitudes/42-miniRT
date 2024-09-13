# Probabulity density functions
- sphere_pdf:
Purpose: Represents a uniform PDF over the unit sphere.
value Method: Returns a constant value of 1/(4π), indicating equal probability for all directions.
generate Method: Generates a random unit vector using the random_unit_vector function.

- cosine_pdf:
Purpose: Represents a PDF that favors directions aligned with a given axis (cosine-weighted).
value Method: Calculates the PDF value based on the cosine of the angle between the direction and the specified axis.
generate Method: Generates a random direction aligned with the specified axis, using the random_cosine_direction function.

- hittable_pdf:
Purpose: Represents a PDF based on the probability of hitting an object in a scene.
value Method: Delegates the PDF calculation to the objects object, using its pdf_value method.
generate Method: Generates a random direction by randomly selecting an object in the scene and sampling a direction from that object's surface.

## Lambert's Cosine Law


## ONB 
> An orthonormal basis (ONB) is a collection of three mutually orthogonal unit vectors. It is a strict subtype of coordinate system. The Cartesian xyz axes are one example of an orthonormal basis.  

In code we create a struct and an initializer.

## cotangent vector

Cotangent vectors are used in computer graphics for texture mapping and shading calculations.

If a tangent vector to a curve at a point is a vector that points in the direction of the curve at that point. It is often used to represent the velocity or direction of motion along the curve.

Cotangent Vector: A cotangent vector is a dual vector to the tangent vector. It is a linear functional that takes tangent vectors as input and returns a scalar value. In essence, it is a function that maps tangent vectors to numbers.

As an example with a particle moving through space:  
While the tangent vector represents the direction of motion, the cotangent vector represents the "force" acting on the particle in that direction.  

Let $F$ be the force acting on the particle.
Cotangent Vector: The cotangent vector ω can be defined as $ω(v) = F · v$, where $·$ denotes the dot product.
Physical Interpretation: $ω(v)$ represents the component of the force that acts in the direction of the particle's velocity.

## The Phong Reflection Model
- Ambient reflection is background lighting, or light reflected from other
objects in the environment. The Phong model treats this as a constant,
coloring all points on the surface equally.
- Diffuse reflection is light reflected from a matte surface. It depends only
on the angle between the light source and the surface normal.
- Specular reflection is the reflection of the light source itself and results in
what is called a specular highlight—the bright spot on a curved surface.
It depends only on the angle between the reflection vector and the eye
vector and is controlled by a parameter that we’ll call shininess. The
higher the shininess, the smaller and tighter the specular highlight.

example from the ray challenge book

Scenario: Lighting with eye opposite surface, light offset 45°
Given eyev ← vector(0, 0, -1)
And normalv ← vector(0, 0, -1)
And light ← point_light(point(0, 10, -10), color(1, 1, 1))
what is the result? 

The provided code snippet seems to be a simplified implementation of a lighting model in computer graphics, specifically for a diffuse material. Let's break down the calculations and the resulting color:

**Given Values:**

- `eyev`: The eye vector, pointing directly towards the surface (0, 0, -1).
- `normalv`: The surface normal vector, also pointing directly towards the surface (0, 0, -1).
- `light`: A point light source located at (0, 10, -10) with color (1, 1, 1).
- `m`: A material with properties:
  - `color`: (1, 1, 1) (white)
  - `ambient`: 0.1 (amount of ambient light reflected)
  - `diffuse`: 0.9 (amount of diffuse light reflected)
  - `specular`: 0.9 (amount of specular light reflected)
  - `shininess`: 200.0 (controls the sharpness of the specular highlight)

- Calculate the vector from the surface point (`position`) to the light source: `light_dir = light.position - position`.
- Normalize the light direction vector: `light_dir = normalize(light_dir)`.
- Calculate the dot product between the surface normal and the light direction: `dot(normalv, light_dir)`.
- If the dot product is negative, the light source is behind the surface, so the diffuse component is 0.
- Otherwise, multiply the dot product by the material's diffuse coefficient (0.9) and the light's intensity (1) to get the diffuse component.
- Multiply the material's ambient coefficient (0.1) by the light's intensity to get the ambient component.
- Add the diffuse and ambient components together to get the final color.


- The dot product between `normalv` and `light_dir` will be 0.7071 (cosine of 45 degrees).
- The diffuse component will be 0.9 * 1 * 0.7071 = 0.6364.
- The ambient component will be 0.1 * 1 = 0.1.
- The total color will be 0.6364 + 0.1 = 0.7364.

The final color is a combination of diffuse and ambient light. The diffuse component is significant due to the 45-degree angle between the light source and the surface, resulting in a moderate amount of diffuse reflection. The ambient component adds a constant background lighting level.



