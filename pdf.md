# Probability density functions
- sphere_pdf: Represents a uniform PDF over the unit sphere.
value func: Returns a constant value of 1/(4π), indicating equal probability for all directions.  
generate func: Generates a random unit vector using the random_unit_vector function.

- cosine_pdf:
Represents a PDF that favors directions aligned with a given axis (cosine-weighted).
value func: Calculates the PDF value based on the cosine of the angle between the direction and the specified axis.
generate func: Generates a random direction aligned with the specified axis, using the random_cosine_direction function.

- hittable_pdf:
Represents a PDF based on the probability of hitting an object in a scene.  
value func: Delegates the PDF calculation to the hittablelist object, using its pdf_value func.
generate func: Generates a random direction by randomly selecting an object in the scene and sampling a direction from that object's surface.

## Lambert's Cosine Law
Describes the relationship between the intensity of light emitted or reflected by a surface and the angle at which it is observed. Specifically, it states that the radiant intensity observed from an ideal diffuse surface (Lambertian surface) is directly proportional to the cosine of the angle θ between the direction of the incident light and the surface normal.
Mathematically, it can be expressed as:

$$I(\theta) = I_0 \cos(\theta)$$

Where:

$I(\theta)$ is the observed intensity when the light shines at angle $θ$.
$I_0$ is the intensity when the light is perpendicular to the surface $θ = 0$.  
$θ$ is the angle between the incident light direction and the surface normal.
In simpler terms, Lambert's Cosine Law implies that the brightness of a Lambertian surface appears the same from all viewing angles, as long as the surface is illuminated uniformly.

## ONB 
> An orthonormal basis (ONB) is a collection of three mutually orthogonal unit vectors. It is a strict subtype of coordinate system. The Cartesian xyz axes are one example of an orthonormal basis.  

In code we create a struct and an initializer for it.  
[...]

## cotangent vector

Cotangent vectors are used in computer graphics for texture mapping and shading calculations.

A tangent vector to a curve at a point is a vector, that points in the direction of the curve at that point. It is often used to represent the velocity or direction of motion along the curve.

A cotangent vector is a dual vector to the tangent vector. It is a linear functional that takes tangent vectors as input and returns a scalar value. In essence, it is a function that maps tangent vectors to numbers.

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

example from the ray challenge book:  
```pseudocode
Scenario: Lighting with eye opposite surface, light offset 45°
Given eyev ← vector(0, 0, -1)
And normalv ← vector(0, 0, -1)
And light ← point_light(point(0, 10, -10), color(1, 1, 1))
what is the result? 
```

- `eyev`: The eye vector, pointing directly towards the surface (0, 0, -1).
- `normalv`: The surface normal vector, also pointing directly towards the surface (0, 0, -1).
- `light`: A point light source located at (0, 10, -10) with color (1, 1, 1).
- `m`: A material with properties:
  - `color`: (1, 1, 1) (white)
  - `ambient`: 0.1 (amount of ambient light reflected)
  - `diffuse`: 0.9 (amount of diffuse light reflected)
  - `specular`: 0.9 (amount of specular light reflected)
  - `shininess`: 200.0 (controls the sharpness of the specular highlight)

Let's calculate the vector from the surface point (`position`) to the light source: `light_dir = light.position - position`.
- We normalize the light direction vector: `light_dir = normalize(light_dir)`.
- We calculate the dot product between the surface normal and the light direction: `dot(normalv, light_dir)`.
- If the dot product is negative, the light source is behind the surface, so the diffuse component is 0.
- Otherwise, we multiply the dot product by the material's diffuse coefficient (0.9) and the light's intensity (1) to get the diffuse component.
- Multiply the material's ambient coefficient (0.1) by the light's intensity to get the ambient component.
- Add the diffuse and ambient components together to get the final color.

Results:  
- The dot product between `normalv` and `light_dir` will be 0.7071 (cosine of 45 degrees).
- The diffuse component will be 0.9 * 1 * 0.7071 = 0.6364.
- The ambient component will be 0.1 * 1 = 0.1.
- The total color will be 0.6364 + 0.1 = 0.7364.

The final color is a combination of diffuse and ambient light.  

## what is the hittable_pdf
The `hittable_pdf` in the book "Raytracing: The Rest of Your Life" is a probability density function (PDF) that represents the likelihood of a ray hitting a particular object in a scene. It's used in conjunction with importance sampling to improve the efficiency of raytracing, especially for complex scenes with many objects.


- Hittable: Refers to any object in the scene that a ray can potentially intersect.
- PDF: A function that describes the probability of a random variable taking on a particular value. In this case, the random variable is the direction of a ray.
- Importance Sampling: A technique that biases the sampling of rays towards areas of the scene that are more likely to contribute significantly to the final image.

How `hittable_pdf` Works:

1. The `hittable_pdf` is initialized with a list of hittable objects and a point of origin.
2. `value` func: This func calculates the probability of a ray originating from the given point and traveling in a specific direction hitting any of the objects in the list.
3. `generate` func: This func generates a random direction based on the `hittable_pdf`. It selects a random object from the list and samples a direction from that object's surface, weighted according to the object's area and distance from the origin.

Why `hittable_pdf` is important:

- Efficient Sampling: By using `hittable_pdf` for sampling, rays are more likely to be directed towards objects that are more likely to contribute to the final image, reducing wasted samples.
- Realistic Lighting: Accurate sampling of objects helps to achieve more realistic lighting effects, especially for complex scenes with many objects and occlusions.


We use it to:  
- Sample light sources more efficiently.  
- Generate scattered rays for indirect lighting calculations.  
- Improve the accuracy of global illumination techniques.  

## what is sphere_pdf

It is a pdf to sample light sources that are spherical in shape.
Generate scattered rays from a spherical object.
Improve the accuracy of global illumination calculations involving spheres.

The sphere_pdf being $1 / (4 * PI)$ for a sphere indicates that the probability of a ray hitting any particular point on the sphere's surface is uniform. This means that every point on the sphere has an equal chance of being hit by a ray.

## Cosine PDF
A cosine PDF is commonly used to represent flat light sources in raytracing.

This is because a perfectly diffuse surface, such as a flat light source, emits light with a cosine distribution. This means that the intensity of light emitted in a particular direction is proportional to the cosine of the angle between that direction and the surface normal.

Key Points:

Cosine Distribution: A cosine distribution means that light is emitted more strongly in directions that are closer to the surface normal and less strongly in directions that are more tangential.  
Flat Light Sources: Idealized flat light sources, like a ceiling light or a wall lamp, are often assumed to have a cosine emission pattern.  

Using Cosine PDF for Flat Light Sources:  

- Initialization: A cosine PDF is typically initialized with the surface normal of the flat light source.
- value func: This func calculates the probability density for a given direction based on the cosine of the angle between the direction and the surface normal.
- generate func: This func generates a random direction according to the cosine PDF, favoring directions closer to the surface normal.

Benefits:

Realistic Lighting: Using a cosine PDF for flat light sources accurately models their diffuse emission pattern, leading to more realistic lighting effects.  
Efficient Sampling: By sampling light rays according to the cosine PDF, you can focus your sampling efforts on directions that are more likely to contribute significantly to the final image.  

In our code:  
```c
double cosine_pdf_value(const void *self, const t_vec3 *direction)
{
	t_cosine_pdf *cos_pdf = (t_cosine_pdf *)self;
	double cosine_theta = dot(unit_vector(*direction), cos_pdf->uvw.w);
	return (fmax(0, cosine_theta / PI));
}
```

`unit_vector(*direction)` normalizes the input direction vector.  
`cos_pdf->uvw.w` represents the w component of the orthonormal basis (ONB) associated with the cosine PDF.   This component is typically aligned with the surface normal.  
`dot(...)` calculates the dot product between the normalized direction vector and the `w` component. This gives the cosine of the angle between the direction and the surface normal.  
The `fmax` function ensures that the returned value is non-negative.  
The result is divided by PI to normalize the PDF, ensuring that the integral of the PDF over all directions equals 1.  

The `cosine_pdf_generate` function is responsible for generating a random direction according to a cosine probability density function (PDF). Used to simulate diffuse materials, where light is scattered uniformly in all directions with a bias towards the surface normal.

Breakdown of the Function:

1. Type Casting:
   - `t_cosine_pdf *cos_pdf = (t_cosine_pdf *)self;`: This line casts the `self` pointer to a `t_cosine_pdf` pointer. 

2. ONB Transformation:
   - `onb_transform(&cos_pdf->uvw, random_cosine_direction())`: This line transforms a random direction generated by `random_cosine_direction()` into the local coordinate system defined by the `uvw` orthonormal basis (ONB) associated with the cosine PDF.

`random_cosine_direction()`:
- This function generates a random direction vector within a unit hemisphere centered at the origin, with a bias towards the z-axis. This bias is consistent with the cosine distribution.


## dot product
The dot product doesn't give you an orthogonal vector; it gives you a scalar. This scalar value represents the projection of one vector onto another, scaled by their magnitudes.

Geometric Interpretation:

- Imagine two vectors, `A` and `B`.
- Project `B` onto `A`. The length of this projection is the scalar value obtained from the dot product.
- If the angle between `A` and `B` is 90 degrees (they are orthogonal), the projection of one onto the other is 0. This is why the dot product of orthogonal vectors is 0.

Mathematical Formula:

The dot product of two vectors `A` and `B` is given by:

```
A · B = |A| * |B| * cos(θ)
```

where:

- `|A|` and `|B|` are the magnitudes of vectors `A` and `B`, respectively.
- `θ` is the angle between vectors `A` and `B`.

Cosine Relationship:
The dot product is directly related to the cosine of the angle between the two vectors. When the angle is 0 (vectors are parallel), the cosine is 1, and the dot product is maximum. When the angle is 90 degrees (vectors are orthogonal), the cosine is 0, and the dot product is 0.

## Understanding the Cross Product
The cross product is a mathematical operation that takes two vectors as input and produces a third vector that is perpendicular to both of them. This resulting vector is often referred to as the normal vector.

Imagine two vectors, A and B, in three-dimensional space. The cross product A × B produces a new vector that is perpendicular to the plane containing A and B. The direction of this new vector is determined by the right-hand rule: if you curl your right hand from A towards B, your thumb will point in the direction of the cross product.

Mathematical Formula:

The cross product of two vectors $A = (a1, a2, a3)$ and $B = (b1, b2, b3)$ is given by:

$$A × B = (a2 * b3 - a3 * b2, a3 * b1 - a1 * b3, a1 * b2 - a2 * b1)$$

Properties of the Cross Product:

- Anti-commutativity: $A × B = -B × A$  
- Distributivity: $A × (B + C) = A × B + A × C$
- Scalar multiplication: $(cA) × B = c(A × B) = A × (cB)$
- Magnitude: $|A × B| = |A| * |B| * sin(θ)$, where $θ$ is the angle between $A$ and $B$.

Applications of the Cross Product:
- Finding the normal vector to a plane: Given two vectors in the plane, their cross product gives the normal vector.
- Calculating torque: In physics, the torque of a force on an object is calculated using the cross product.
- Determining the direction of rotation: In computer graphics and robotics, the cross product is used to determine the direction of rotation.

Example:
If A = (1, 2, 3) and B = (4, 5, 6), then:  
A × B = (2 * 6 - 3 * 5, 3 * 4 - 1 * 6, 1 * 5 - 2 * 4) = (-3, 6, -3)  
The vector (-3, 6, -3) is perpendicular to both A and B.  

# A plane
## Creating a Plane Initializer in Raytracing

Understanding the Plane Structure:

A plane can be defined by a point on the plane and its normal vector. The equation of a plane is typically represented as:

$$Ax + By + Cz + D = 0$$

where (A, B, C) is the normal vector and D is a constant.  
Initializing the Plane Structure in our code:

```c
t_plane plane(t_point3 p, t_vec3 n, t_rgb rgbcolor) {
    t_plane pl;

    pl.base.hit = hit_plane;
    pl.base.pdf_value = plane_pdf_value;
    pl.base.random = plane_random;
    pl.p = p;
    pl.n = unit_vector(n);
    pl.d = -dot(pl.n, p); // Calculate D for the plane equation
    pl.rgb = rgbcolor;
    pl.color = rgb_to_color(rgbcolor);
    solid_color_init(&(pl.texture), pl.color);
    lambertian_init_tex(&(pl.lambertian_mat), (t_texture*)&(pl.texture));
    pl.mat = (t_material*)&(pl.lambertian_mat);

    return pl;
}
```

- Normal Vector: The `n` member is directly used as the normal vector of the plane.
- Plane Equation Constant: The `d` member is calculated using the dot product of the normal vector and the point on the plane, ensuring that the plane equation is satisfied.
- Other Members: The remaining members (like `rgb`, `color`, `texture`, `mat`) are initialized similarly to the other structures. We use a default material with a Lambertian texture for our objects.

## creating a triangle
## Creating a Triangle Initializer

Understanding the Triangle Structure:

A triangle can be defined by three vertices. The normal vector of the triangle can be calculated by taking the cross product of two edges.

Triangle Initializer:

```c
t_triangle triangle_rgb(t_point3 a, t_point3 b, t_point3 c, t_rgb rgbcolor) {
    t_triangle tri;

    tri.base.hit = hit_triangle;
    tri.base.pdf_value = triangle_pdf_value;
    tri.base.random = triangle_random;
    tri.a = a;
    tri.b = b;
    tri.c = c;
    tri.rgb = rgbcolor;
    tri.color = rgb_to_color(rgbcolor);
    tri.normal = unit_vector(cross(vec3substr(b, a), vec3substr(c, a)));
    tri.d = dot(tri.normal, a);
    tri.area = 0.5 * length(cross(vec3substr(b, a), vec3substr(c, a)));

    // Initialize texture and material as before
    solid_color_init(&(tri.texture), tri.color);
    lambertian_init_tex(&(tri.lambertian_mat), (t_texture*)&(tri.texture));
    tri.mat = (t_material*)&(tri.lambertian_mat);

    return tri;
}
```

- Vertices: The triangle is defined by three vertices `a`, `b`, and `c`.
- Normal: The normal vector is calculated using the cross product of the vectors `b - a` and `c - a`.
- Area: The area of the triangle is calculated using the formula `0.5 * |a - b| * |c - a| * sin(θ)`, where θ is the angle between the vectors `a - b` and `c - a`. However, since we're dealing with a triangle in a plane, the sin(θ) term is always 1, so we can simplify it to `0.5 * |cross(a - b, c - a)|`.

Hit Function for Triangle:

This can be done using barycentric coordinates to check if the intersection point lies within the triangle's boundaries.

## Hitting a Triangle in Raytracing

Barycentric Coordinates:

Barycentric coordinates are a convenient way to represent a point within a triangle relative to its vertices. A point P can be expressed as a linear combination of the triangle's vertices A, B, and C using barycentric coordinates (u, v, w):

```
P = uA + vB + wC
```

The condition for P to lie within the triangle is:

```
0 <= u <= 1
0 <= v <= 1
0 <= w <= 1
u + v + w = 1
```

Modified `hit_triangle` Function:

```c
bool hit_triangle(const void* self, const t_ray *r, t_interval ray_t,  t_hit_record *rec) {
       const t_triangle *tri = (t_triangle *)self;
    t_vec3 e1 = tri->edge1;
    t_vec3 e2 = tri->edge2;

        // the .. Trumbore algo
    
    // Calculate the ray direction vector
    t_vec3 dir_cross_e2 = cross(r->dir, e2);

    // Calculate the determinant
    double det = dot(e1, dir_cross_e2);

    if (fabs(det) < EPSILON)
        return false; // Ray is parallel to the triangle

    // Calculate barycentric coordinates
    double f = 1.0 / det;
    
    t_vec3 p1_to_origin = vec3substr(r->orig, tri->a);
    
    double u = f * dot(p1_to_origin, dir_cross_e2);
    if (u < 0 || u > 1)
        return false;
    
    t_vec3 origin_cross_e1 = cross(p1_to_origin, e1);
    double v = f * dot(r->dir, origin_cross_e1);
    
    if (v < 0 || u + v > 1)
        return false;


    // Calculate the intersection point
    double t = f * dot(e2, origin_cross_e1);

// populate the hit record
    rec->t = t;
    rec->p = point_at(r, t);
    rec->mat = tri->mat;
    set_face_normal(rec, r, tri->normal);

    return true;
}
```


- We calculate the barycentric coordinates `u` and `v` of the intersection point relative to the triangle's vertices.
- If both `u` and `v` are non-negative and their sum is less than or equal to 1, the intersection point lies within the triangle.

This modified `hit_triangle` function accurately determines if a ray intersects a triangle and sets the hit record accordingly.

## triangle pdf functions
The `triangle_pdf_value` function for a triangle can be derived from the general formula for the probability density function of a point on a 2D surface:

```
pdf(x, y) = 1 / area
```

where `area` is the area of the surface.

For a triangle, the area can be calculated using the cross product of two edges:

```
area = 0.5 * |cross(e1, e2)|
```

where `e1 = b - a` and `e2 = c - a` are the vectors representing two edges of the triangle.

Combining these equations, we get:

```c
double triangle_pdf_value(const void *self, const t_point3 *orig, const t_vec3 *dir) 
{
    const t_triangle *tri = (t_triangle *)self;
    t_hit_record rec;

    const t_ray r = ray(*orig, *dir);
    if (!hit_triangle(tri, &r, interval(0.001, INFINITY), &rec))
        return 0;

    double distance_squared = length_squared(vec3substr(rec.p, *orig));
    double cosine = fabs(dot(*dir, tri->normal));

    // Calculate the area of the triangle
    t_vec3 e1 = vec3substr(tri->b, tri->a);
    t_vec3 e2 = vec3substr(tri->c, tri->a);
    double area = 0.5 * length(cross(e1, e2));

    return distance_squared / (cosine * area);
}
```

This function calculates the PDF for a ray hitting a specific point on the triangle's surface. It takes into account the distance from the origin, the angle between the ray and the triangle's normal, and the area of the triangle.
