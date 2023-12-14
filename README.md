# "The All Americans" 4D Ray Tracer

Final Project for CS1230 by Nick Bottone, Lafayette Bussey, David Doan, and Michael Foiani.

[![youtube thumbnail](https://markdown-videos-api.jorgenkh.no/youtube/xtDSMKPcBYY)](https://youtu.be/xtDSMKPcBYY)

## Project Description

A raytracer capable of loading and rendering arbitrary scenes with four dimensions of space and one dimension of time. The raytracer supports the following features:
* Rendering of tesseracts (hypercubes), hyperspheres, hypercylinders, and hypercones
* Directional lights, point lights, and ambient lighting shaded with the Phong illumination model
* 4D reflections and refractions
* Collision detection between hyperspheres and hypercubes using specific extensions of the split axis theorem
* Translating camera in all 4d dimensions, with rotations in 3d
* Camera movement on a Bezier curve
* GUI sliders to offer realtime interactive functionality
* Animation with bulk rendered outputs and videos

## Setup and Installation

FFmpeg and Cmake with Qt5 are required prerequisites in order to run this project.

## Building and Running the Project

To build the project, run the following commands from the root directory of the project:

```bash
cmake --build ./build/ --config Release --target all -j 10 --
```

You can also build with Debug mode by replacing `Release` with `Debug` in the above command. Release mode is recommended for faster rendering. Running the build command again is only necessary if you make changes to the source code.

To run the project, run the following commands from the root directory of the project:

```bash
./build/projects_ray
```

## Usage

The GUI is split into two sections: the left side is the menu containing sliders that control the camera and the animation timeline, and the right side is the render window. The render window will display the rendered image. You can use the buttons on the left side to import a scenefile, save a single rendered frame as an image, or trigger a bulk export of an animated video. The sliders also have the ability to control the camera position and rotation, as well as the `w` parameter determining which 3d subspace the camera is viewing. 

![screenshot of raytracer user interface](https://i.imgur.com/soqP2L3.png)

## Scenefile Format

A number of 4D scenefiles are provided in the [`scenefiles-4d/`](scenefiles-4d/) directory. The scenefiles follow a JSON format where each scene contains some global constants, some camera data, and a number of nested objects. These nested objects can contain transformations, and may include lights or primitives. Take a look at any of the provided scenefiles for examples of how to structure a scene.
