# Parser syntax for .rt files

The `.rt` files follow our conventions. The following is a list of the syntax used in the `.rt` files.

## Ambient light
Inits the ambient struct inside of t_mrt->t_camera.  
usage:
```
"A" [intensity([0.0;1.0])] [rgb color]
```


## Camera
Inits the camera struct inside of t_mrt.
usage:
```
"C" [origin] [orientation(view) vector (normalized values)] [horiz. fow (double)]
```

## Light

The defaut light is a sphere or a quad with a light texture.
usage (for default sphere light):
```
l [origin] [intensity([0.0;1,0])] [rgb color] [optional : diameter(default value=100)]
```


Quad light source.  
usage:
``` 
"l" "qd" [origin] [side_vector1] [side_vector2] [rgb color] [intensity ([0.0;1.0])]
```

## Sphere
Creates a sphere object.
usage:  
default sphere -		`"sp" [origin] [diameter] [rgb color]`
checker texture -	`"sp" [origin] [diameter] [rgb color1] [rgb color2]`
image (earthmap) -	`"sp" [origin] [diameter] "img:"[path to .jpg]`
metal sphere -		`"sp" [origin] [diameter] [rgb color] [fuzz value double)]`

```
sp [origin] [diameter] [rgb color]
sp [origin] [diameter] [rgb color1] [rgb color2]
sp [origin] [diameter] "img:"[path to .jpg]
sp [origin] [diameter] [rgb color] [fuzz value(double)]
```

## Plane
Creates a plane object.
usage:
```
pl [origin] [surface normal ([0;1],[0;1],[0;1])] [rgb color]
pl [origin] [surface normal ([0;1],[0;1],[0;1])] [rgb color] [fuzz(double)]
pl [origin] [surface normal ([0;1],[0;1],[0;1])] [rgb color1] [rgb color2]
```

## Cylinder
usage:

```
cy [origin] [axis normal] [diameter] [height] [rgb color]
cy [origin] [axis normal] [diameter] [height] [rgb color] [fuzz(double)]
```

## Quad
Creates a quad object.
usage:
```
q [origin] [side_vector1] [side_vector2] [rgb color]
q [origin] [side_vector1] [side_vector2] [rgb color] [fuzz(double)]
```

## Disk
Creates a disk object.
usage:
```
dsk [origin] [normal] [diameter] [rgb color]
dsk [origin] [normal] [diameter] [rgb color] [fuzz(double)]
```

## Triangle
Creates a triangle object.
usage:
```
tr [vertice1] [vertice2] [vertice3] [rbg color]
tr [vertice1] [vertice2] [vertice3] [rbg color] [fuzz(double)]
```

## Box
/*
 * usage:
 * default - "box" [origin] [diagonal point] [color]
 * metalic - "box" [origin] [diagonal point] [color] [fuzz(double)]
*/
Creates a box object.
usage:
```
box [origin] [diagonal point] [color]
box [origin] [diagonal point] [color] [fuzz(double)]
```

<!-- ## Cone
Creates a cone object.
usage:
```
cone [origin] [axis normal] [diameter] [height] [rgb color]
cone [origin] [axis normal] [diameter] [height] [rgb color] [fuzz(double)]
``` -->