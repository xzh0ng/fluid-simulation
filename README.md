# fluid-simulation (CSC417 Final Project)

### Demo
https://www.youtube.com/watch?v=XkXg6B8tpwc

### Dependencies
* `Eigen`
* `libigl`
* `OpenMP`

### How to compile
Make sure you have libigl to the main folder. Create a build folder and run `cmake -DCMAKE_BUILD_TYPE=Release ..`.
You should have both `Eigen` and `libigl` after cmake.
Go to the build folder and `makefile`. Run `fluid-simulation.exe` or `fluid-simulation`.

### How to run
We have 5 different scenes. You can press `1`, `2`, `3`, `4`, `5` to change scenes.
Press `r` to randomly reset all particle's position in the scene.
Press `w` and `s` to increase and decrease thie size of the particles (visually).


### External resources
The texture used on particles is copied from [Alec Jacobson's blog]https://www.alecjacobson.com/weblog/?p=4827

Bunny mesh is copied from A3's `coarser_bunny.obj` with faces decimated.

### Reference
https://dl.acm.org/doi/10.1145/1073368.1073400
