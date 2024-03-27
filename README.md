# RayTracer
A simple ray tracer written from scratch in C++, with multicore support, customizable camera, attenuation, alpha-beta correction, reflection, shadows, and more!

# Credits
Primary author: Jeffrey Persons

Additional Contributions by [@DerekMcBlane](https://github.com/derek-mcblane):
* Bug Fixes: Fixed false positive sphere intersections, and leaky reflections that bled through
* Infrastructure Improvements: Addition of test (gtest) and build systems (cmake)


## Current Features
* Parallelized tracing algorithm using OpenMP
* Perspective, axis aligned camera with lookAt functionality
* Attenuation, specular, and diffuse lighting implemented via phong shading
* Material, Color, vector, and geometric primitives
*	Fully modularlized tracing pipeline with Camera, Scene, FrameBuffer, virtual interfaces for shape and lighting primitives
* Self contained math libraries for interpolation, vector operations, and ray intersections
*	Stopwatch for benchmarking

## Features I'd Like to Add
### High Priority
* YAML scene discription files
* Polygon meshes
* Softer shadows
* Refraction
* Scripting for creating animations


### Nice to Haves (will add if time)
* Proper refraction with fresnels
* KD-trees for fast rendering and culling with tons of on screen-objects
* Radiance 


# Gallery - Version 3
*(click on below images to view full size in a seperate tab)*
<p align="center">
  <img src="https://raw.githubusercontent.com/jeffreypersons/Jeff-Images/master/ray-tracing-revamped/ray-traced-scene.png" alt="Example Scene_front-top-view" width="500"/>
</p>


# Gallery - Version 2
*(click on below images to view full size in a seperate tab)*
<p align="center">
  <img src="https://raw.githubusercontent.com/jeffreypersons/Jeff-Images/master/ray-tracing-revamped/scene-front-top.png" alt="Example Scene_front-top-view" width="500"/>
</p>
<p align="center">
  <img src="https://raw.githubusercontent.com/jeffreypersons/Jeff-Images/master/ray-tracing-revamped/scene-top.png" alt="Example Scene_top-view" width="500"/>
</p>

# Gallery - Version 1
*(click on below image to view full size in a seperate tab)*
<p align="center">
  <img src="https://raw.githubusercontent.com/jeffreypersons/Jeff-Images/master/scene.png" alt="Example Scene" width="500"/>
</p>


## Build & Run Instructions
#CMake on Unix
Build:
`./build.sh`
Build output goes to directory named after build type.  Can pass build type as an argument (e.g. `./build.sh Debug`)
Run:
`./Release/bin/App`

