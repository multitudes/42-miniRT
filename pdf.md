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