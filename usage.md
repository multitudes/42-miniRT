# Inputs for the program
The program takes just one argument, and that is the path to a "scene file". The scene file
contains definitons of objects and their properties, which the raytracer is going to render.
The file **must** have the *.rt* extension.

## How to specify objects

### overall:
- Each object is defined on a single line (objects are seperated my newlines).
- The file can have empty lines.
- The file can contain comments (everything after **#** is ignored).
- The properties for each object can be seperated by a mixture of spaces and tabs.

### defining a single object.

Each object has its properties. The order of the properties is crucial.

| Object        | Specifier |                                     |                                                |                                     |                                            |                                     |   |   |   |   |   |   |
|---------------|-----------|-------------------------------------|------------------------------------------------|-------------------------------------|--------------------------------------------|-------------------------------------|---|---|---|---|---|---|
| Camera        | C         | xyz coordinates (float,float,float) | orientation ([0.0-1.0],[0.0-1.0],[0.0-1.0])    | horizontal fov [0,180] degrees      |                                            |                                     |   |   |   |   |   |   |
| Ambient light | A         | light intensity [0.0,1.0]           | rgb color ([0-255],[0-255],[0-255])            |                                     |                                            |                                     |   |   |   |   |   |   |
| light         | l         | xyz coordinates (float,float,float) | light intensity [0.0,1.0]                      | rgb color ([0-255],[0-255],[0-255]) | optional - diam(float) - default diam = 20 |                                     |   |   |   |   |   |   |
| sphere        | sp        | xyz coordinates (float,float,float) | diameter (float)                               | rgb color ([0-255],[0-255],[0-255]) | TODO                                       |                                     |   |   |   |   |   |   |
| plane         | pl        | xyz coordinates (float,float,float) | orientation ([0.0-1.0],[0.0-1.0],[0.0-1.0])    | rgb color ([0-255],[0-255],[0-255]) |                                            |                                     |   |   |   |   |   |   |
| cylinder      | cy        | xyz coordinates (float,float,float) | axis direction ([0.0-1.0],[0.0-1.0],[0.0-1.0]) | diameter (float)                    | height (float)                             | rgb color ([0-255],[0-255],[0-255]) |   |   |   |   |   |   |
| cone          | co        | TODO                                |                                                |                                     |                                            |                                     |   |   |   |   |   |   |
