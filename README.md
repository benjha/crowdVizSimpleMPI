
![image](https://github.com/benjha/crowdVizSimpleMPI/blob/master/Screenshot.png)


## Description

This example renders  sets of characters in a virtual environment using MPI for parallel rendering in multi-GPU and multi-node 
environments. Each GPU renders a crowd, then the framebuffer is captured and the resultant image with depth is sent to the 
master node, no fancy communications are performed such as binary-swap. In the master node sort-last composition using GLSL 
is performed. 

### Dependencies

This project needs the following libraries previously installed:

- ASSIMP version 3. You might need compile ASSIMP's source code in your system in order to get version 3.
- GLEW
- Freeglut
- glm
- OpenMPI
- pthreads

In a cluster environment:

- Any image transport library (e.g. virtualGL) installed in your cluster.


GPU with OpenGL 4.2

### Project Organization

Header/       	- contains the header files
Source/		- contains the source files. The file main.cpp is the entry point of the project
media/		- contains 3D models
shaders/	- contains shaders in GLSL 4.2 


### Compilation

The project was developed using NVIDIA Nsight in LINUX. 

TODO:

In addition you will find the makefiles of the project in the  Debug directory, you can change it according to your needs, however you might need to change *.mk files inside the Debug/Source directory.


### Running 

```
mpi -np <N> <path/name of the executable>
```

where N is the number of nodes, it has been tested for N=1 to 9 nodes.

The app will open N windows, N-1 windows will render NUM_CHARACTERS models and the master window (title Process ID 0) will display 
the composition of the other subwindows. Open the  Screenshot.png file

You can interact with the mouse and WASD keys by previously select the Process ID 0 (master) window. Pressing ESC in this window or closing it, will shut down the app.

#### NOTES

Only tested in LINUX

