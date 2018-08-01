Simulation
==========

A simulation program written in C++.

This was made as an experiment to learn how to use OpenGL in C++ and to experiment with numerical schemes for solving partial differential equations. It uses OpenGL 3 to render a toy model simulation of a beach ball floating on a water surface in 3D. The user can press the Q, W, A and S keys to create waves in real-time.

Author: A.C. Kockx



Requirements
------------

This program requires the following external dependencies to build and run:

* Graphics Library Framework (GLFW) version 3.2.1
* OpenGL Extension Wrangler Library (GLEW) version 2.1.0
* OpenGL Mathematics (GLM) version 0.9.9.0

The header and lib files of these dependencies are included in the "third_party" folder. The dll files of these dependencies are included in the "bin" folder.

In order to run, your system must have a GPU that supports OpenGL 3.

Before running make sure that the "shaders" folder is next to the "bin" folder and that the current working directory is e.g. "bin/x64/" otherwise the program cannot find the shader .glsl files. In other words the shader .glsl files must be in a folder called "shaders" with a path "../../shaders" relative to the current working directory.



Build
-----

Visual Studio 2017 solution and project files are included for building using Visual Studio.
