# Project 4: Illuminate

Please fill this out for Illuminate only. The project handout can be found [here](https://cs1230.graphics/projects/ray/2).

## Output Comparison
Run the program with the specified `.ini` file to compare your output (it should automatically save to the correct path).
> If your program can't find certain files or you aren't seeing your output images appear, make sure to:<br/>
> 1. Set your working directory to the project directory
> 2. Set the command-line argument in Qt Creator to `template_inis/illuminate/<ini_file_name>.ini`
> 3. Clone the `scenefiles` submodule. If you forgot to do this when initially cloning this repository, run `git submodule update --init --recursive` in the project directory

> Note: once all images are filled in, the images will be the same size in the expected and student outputs.

| File/Method To Produce Output | Expected Output | Your Output |
| :---------------------------------------: | :--------------------------------------------------: | :-------------------------------------------------: | 
| point_light_1.ini |  ![](https://raw.githubusercontent.com/BrownCSCI1230/scenefiles/main/illuminate/required_outputs/point_light/point_light_1.png) | ![Place point_light_1.png in student_outputs/illuminate/required folder](student_outputs/illuminate/required/point_light_1.png) |
| point_light_2.ini |  ![](https://raw.githubusercontent.com/BrownCSCI1230/scenefiles/main/illuminate/required_outputs/point_light/point_light_2.png) | ![Place point_light_2.png in student_outputs/illuminate/required folder](student_outputs/illuminate/required/point_light_2.png) |
| spot_light_1.ini |  ![](https://raw.githubusercontent.com/BrownCSCI1230/scenefiles/main/illuminate/required_outputs/spot_light/spot_light_1.png) | ![Place spot_light_1.png in student_outputs/illuminate/required folder](student_outputs/illuminate/required/spot_light_1.png) |
| spot_light_2.ini |  ![](https://raw.githubusercontent.com/BrownCSCI1230/scenefiles/main/illuminate/required_outputs/spot_light/spot_light_2.png) | ![Place spot_light_2.png in student_outputs/illuminate/required folder](student_outputs/illuminate/required/spot_light_2.png) |
| simple_shadow.ini |  ![](https://raw.githubusercontent.com/BrownCSCI1230/scenefiles/main/illuminate/required_outputs/shadow/simple_shadow.png) | ![Place simple_shadow.png in student_outputs/illuminate/required folder](student_outputs/illuminate/required/simple_shadow.png) |
| shadow_test.ini |  ![](https://raw.githubusercontent.com/BrownCSCI1230/scenefiles/main/illuminate/required_outputs/shadow/shadow_test.png) | ![Place shadow_test.png in student_outputs/illuminate/required folder](student_outputs/illuminate/required/shadow_test.png) |
| shadow_special_case.ini |  ![](https://raw.githubusercontent.com/BrownCSCI1230/scenefiles/main/illuminate/required_outputs/shadow/shadow_special_case.png) | ![Place shadow_special_case.png in student_outputs/illuminate/required folder](student_outputs/illuminate/required/shadow_special_case.png) |
| reflections_basic.ini |  ![](https://raw.githubusercontent.com/BrownCSCI1230/scenefiles/main/illuminate/required_outputs/reflection/reflections_basic.png) | ![Place reflections_basic.png in student_outputs/illuminate/required folder](student_outputs/illuminate/required/reflections_basic.png) |
| reflections_complex.ini |  ![](https://raw.githubusercontent.com/BrownCSCI1230/scenefiles/main/illuminate/required_outputs/reflection/reflections_complex.png) | ![Place reflections_complex.png in student_outputs/illuminate/required folder](student_outputs/illuminate/required/reflections_complex.png) |
| texture_cone.ini |  ![](https://raw.githubusercontent.com/BrownCSCI1230/scenefiles/main/illuminate/required_outputs/texture_tests/texture_cone.png) | ![Place texture_cone.png in student_outputs/illuminate/required folder](student_outputs/illuminate/required/texture_cone.png) |
| texture_cone2.ini |  ![](https://raw.githubusercontent.com/BrownCSCI1230/scenefiles/main/illuminate/required_outputs/texture_tests/texture_cone2.png) | ![Place texture_cone2.png in student_outputs/illuminate/required folder](student_outputs/illuminate/required/texture_cone2.png) |
| texture_cube.ini |  ![](https://raw.githubusercontent.com/BrownCSCI1230/scenefiles/main/illuminate/required_outputs/texture_tests/texture_cube.png) | ![Place texture_cube.png in student_outputs/illuminate/required folder](student_outputs/illuminate/required/texture_cube.png) |
| texture_cube2.ini |  ![](https://raw.githubusercontent.com/BrownCSCI1230/scenefiles/main/illuminate/required_outputs/texture_tests/texture_cube2.png) | ![Place texture_cube2.png in student_outputs/illuminate/required folder](student_outputs/illuminate/required/texture_cube2.png) |
| texture_cyl.ini |  ![](https://raw.githubusercontent.com/BrownCSCI1230/scenefiles/main/illuminate/required_outputs/texture_tests/texture_cyl.png) | ![Place texture_cyl.png in student_outputs/illuminate/required folder](student_outputs/illuminate/required/texture_cyl.png) |
| texture_cyl2.ini |  ![](https://raw.githubusercontent.com/BrownCSCI1230/scenefiles/main/illuminate/required_outputs/texture_tests/texture_cyl2.png) | ![Place texture_cyl2.png in student_outputs/illuminate/required folder](student_outputs/illuminate/required/texture_cyl2.png) |
| texture_sphere.ini |  ![](https://raw.githubusercontent.com/BrownCSCI1230/scenefiles/main/illuminate/required_outputs/texture_tests/texture_sphere.png) | ![Place texture_sphere.png in student_outputs/illuminate/required folder](student_outputs/illuminate/required/texture_sphere.png) |
| texture_sphere2.ini |  ![](https://raw.githubusercontent.com/BrownCSCI1230/scenefiles/main/illuminate/required_outputs/texture_tests/texture_sphere2.png) | ![Place texture_sphere2.png in student_outputs/illuminate/required folder](student_outputs/illuminate/required/texture_sphere2.png) |


## Design Choices
To orgainze the different pieces of code, I categorized every source file into it's own section, all under the RayTracer namespace (to avoid circular dependencies).
In cases where there were switch cases based on the type of primitive, I created a helper function for each non-trivial switch case block to neatly organize those functions that would take up tons of lines.

Other than the organizational structure, no notable design choices.

## Collaboration/References
* refraction -> https://graphics.stanford.edu/courses/cs148-10-summer/docs/2006--degreve--reflection_refraction.pdf
* kd-tree -> https://graphics.stanford.edu/papers/i3dkdtree/gpu-kd-i3d.pdf
* bvh -> https://jacco.ompf2.com/2022/04/13/how-to-build-a-bvh-part-1-basics/
* depth of field -> https://courses.cs.washington.edu/courses/csep557/99au/projects/trace/depthoffield.doc


## Known Bugs
No known bugs with the core assignment (some of the extra credit may be buggy).

## Extra Credit
1) soft shadows (working as intended)
2) depth of field (working as intended)
3) refraction (working as close to intended as possible)
4) bvh acceleration (working as intended)
5) kd-tree (not working, see figures)

Also, all extra credit from intersect is included and actually binded to the ini file. 

### soft shadows
Created a new area light type, which is parsed as normal form the .json scene data (see soft_shadow.json). See illuminateAreaLight under illuminate.cpp. 
Image below shows how the soft shadow effect works (area light points from right side of image).
![](student_outputs/illuminate/extra-credit/soft_shadow.png)
### depth of field
Implemented depth of field using "stochastic" (random multiple-sample) rays over the aperature from the origin in the camera space to mimic the optics of a lens.
See secondaryRays() in raytracer.cpp.
The image below shows the depth of field effect, with the one point a distance from the camera offering the highest resolution, versus the closer and further parts of the image, which progressively get more burry the further (or closer) to the camera.
![](student_outputs/illuminate/extra-credit/dof.png)

### refraction
Implemented refraction using snell's law and frensel's equations. See refract in reflect.cpp for implementation.
Compared to the TA example, the colors do seem a little too dark (possibly due to fransel's), but the accuracy of the refracted angles is perfect 1-to-1 to your example.
![](student_outputs/illuminate/extra-credit/refract1.png)

![](student_outputs/illuminate/extra-credit/refract2.png)
### bvh acceleration
Implemented a bvh tree and a traverse algorithm to more quickly determine if a ray intersects a primitive. Both tree and algorithm work as intended.
My bvh is quicker than the non-accelerated types, but not by much. It could be that there aren't enough object in the scene.
See bvh.h, getPixelFromRay() in raytracer.cpp, & isShadows() in shadow.cpp.
![](student_outputs/illuminate/extra-credit/bvh.png)

### kd-tree acceleration
Implemented a kd tree and a traverse algorithm to more quickly determine if a ray intersects a primitive. The traverse algorithm works, but the tree isn't built correctly. The tree narrows too quickly to one object, and my attempts to fix that result in segfaults (no more memory on the heap).
Below is an image of the result from using kd tree acceleration. Sure is fast, especially when you only have to do half the work!
See kdtree.cpp.

![](student_outputs/illuminate/extra-credit/point_light_2-kd-issues.png)


