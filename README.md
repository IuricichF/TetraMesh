# TetraMesh
A C++ library for the topological representation of scalar fields defined on tetrahedral meshes
---
####Library Description
***

It implements the IA data structure, originally published in:

```
A. Paoluzzi, F. Bernardini, C. Cattani, and V. Ferrucci.
Dimension-Independent Modeling with Simplicial Complexes.
ACM Trans. on Graph., 12(1):56–102, 1993.
```

For a brief description of each function please look at source/Mesh.h


---
####Compile
***

The project can be compiled with QTCreator or using the terminal by typing “qmake” and “make” from this folder.

The code has been tested under Linux and Mac only. No external libraries are needed.



---
####Main.cpp
***

The file main.cpp presents some of the basic functionalities of the library.

1. Read a Mesh from a .ts file (```mesh.loadTS(inputFile);```)

2. Read a Mesh from a .raw file (```mesh.loadRAW(inputFile, x,y,z);```)

3. Visit all the topological relations ( VertexEdge, VertexTriangle, etc. )

The input parameters expected are:

1. Input File (.ts or .raw)
  * if the file is in .raw format also the resolution of the dataset is required

Examples:

1. Read the tetrahedral mesh from a .ts file

    ```
    ./TetraMesh filename.ts
    ```

2. Initialize the tetrahedral mesh from a regular grid

    ```
    ./TetraMesh filename.raw xResolution yResolution zResolution
    ```

#####File Formats
######.ts

Simple ASCII file containing the explicit representation of vertices and tetrahedral
```
nV mT              -  number of vertices (nV) and tetrahedra (mT)
x1 y1 z1 f1        -  x y z coordinates for each vertex and field value
x2 y2 z2 f2
.  .  .  .
xn yn zn fn

v11 v21 v31 v41    -  vertices (v1 v2 v3 v4) of the first tetrahedron
v12 v22 v32 v42
.   .   .   .
v1m v2m v3m v4m

```

######.raw

Binary file containing the field values of the vertices of a regular grid of dimension x*y*z.

The grid resolution must be provided in input


---
####Attribution
***

If you use our library in your project we encourage you to cite our work

```
  @misc{TetraMesh,
    author       = {L. De Floriani, R. Fellegara and F. Iuricich},
    title        = {TetraMesh library},
    howpublished = {\url{https://github.com/IuricichF/TetraMesh}},
    month        = september,
    year         = 2015
  }
```
