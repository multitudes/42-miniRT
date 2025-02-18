#  ------------ Parser syntax for .rt files ------------

> The `.rt` files follow our conventions. The following is a list of the syntax used in the `.rt` files.

-----
## Camera
Inits the camera struct inside of t_mrt.	(MUST BE INITED)

usage:
```
C [origin] [orientation(view) vector (normalized values)] [fow (double)]
```
*fow* - horizontal field of view

-----
## Ambient light
Inits the ambient struct inside of t_mrt->t_camera.  
usage:
```
A [intensity([0.0;1.0])] [rgb color]
```
-----
## Light

The defaut light is a sphere or a quad with a light texture. ("l" or "L" can be used)

### default sphere light:
```
l [origin] [intensity([0.0;1,0])] [rgb color] [optional : diameter(default value=100)]
```

### quad light source:
``` 
l qd [origin] [side_vector1] [side_vector2] [rgb color] [intensity ([0.0;1.0])]
```
-----
## Sphere
###  default sphere:
```
sp [origin] [diameter] [rgb color]
```
### checker texture:
```
sp [origin] [diameter] [rgb color1] [rgb color2]
```
### image overlay:
```
sp [origin] [diameter] "img:"[path to .jpg]
```
### metal
```
sp [origin] [diameter] [rgb color] [fuzz value(double)]
```
-----
## Plane
### default:
```
pl [origin] [surface normal ([0;1],[0;1],[0;1])] [rgb color]
```
### metal (mirror):
```
pl [origin] [surface normal ([0;1],[0;1],[0;1])] [rgb color] [fuzz(double)]
```
### checker:
```
pl [origin] [surface normal ([0;1],[0;1],[0;1])] [rgb color1] [rgb color2]
```
-----
## Cylinder
### default:
```
cy [center] [axis normal] [diameter] [height] [rgb color]
```
### metal:
```
cy [center] [axis normal] [diameter] [height] [rgb color] [fuzz(double)]
```
*center* - the point in the middle of the axis.

-----
## Quad
### default:
```
qd [origin] [side_vector1] [side_vector2] [rgb color]
```
### metal:
```
qd [origin] [side_vector1] [side_vector2] [rgb color] [fuzz(double)]
```
-----
## Disk
### default:
```
dsk [origin] [normal] [diameter] [rgb color]
```
### metal:
```
dsk [origin] [normal] [diameter] [rgb color] [fuzz(double)]
```
-----
## Triangle
### default:
```
tr [vertice1] [vertice2] [vertice3] [rbg color]
```
### metal:
```
tr [vertice1] [vertice2] [vertice3] [rbg color] [fuzz(double)]
```
*vertices* are points in 3d space

-----
## Box
### default:
```
box [origin] [diagonal point] [rgb color]
```
### metal:
```
box [origin] [diagonal point] [rgb color] [fuzz(double)]
```
*diagonal point* is the cube vertice furthest from the origin

---
## Cone
### default:
```
co [apex] [axis normal] [diameter] [height] [rgb color]
```
### metal:
```
co [origin] [axis normal] [diameter] [height] [rgb color] [fuzz(double)]
```

*apex* - the top point of the cone (the tip)

*axis* - axis normal vector going from the apex up

*diameter* - diameter of the base
