CONTROLS
--------
<UP>      - Increase speed of camera
<DOWN>    - Decrease speed of camera
<SPACE>   - Completely stop camera movement
<LEFT>    - Rotate camera left
<RIGHT>   - Rotate camera right
<PG UP>   - Move camera up
<PG DOWN> - Move camera down
P         - Move to preset location
T         - Begin tour
E         - Exit tour mode
Q / <ESC> - Quit

FILES
-----
main.cpp - The entry point for the program. This contains the majority of the program's code, including input handling, loading meshes and textures, and handling the tour.
shader.hpp/shader.cpp - A wrapper class for GLSL shader objects to make loading and using them more convenient.
program.hpp/program.cpp - A similar wrapper class for GLSL programs.
camera.hpp/camera.cpp - Used to calculate the view matrix based on the user's current position and orientation.
mesh.hpp/mesh.cpp - Mesh loader, reads in mesh files and generates a vertex array object and corresponding vertex and index buffers for a given mesh.

The mesh_converter directory contains the source code for my mesh converter, which uses the Assimp library to load model files of various different formats and convert them to my runtime mesh format.
IT IS NOT NECESSARY TO COMPILE THE FILES IN THIS DIRECTORY TO RUN THE PROJECT.
The compiled meshes are included in the meshes directory.

shaders/transform.glsl           - Transforms vertices using the given matrices, and passes coordinates to the fragment shader in eye-space for lighting.
shaders/fullscreen_triangle.glsl - Always emits the vertices of a triangle covering the full screen. This is used to invoke the fragment shader for every pixel on the screen, which is useful for rendering the skybox texture.
shaders/phong.glsl               - Shades fragments using the Phong reflection model. This shader also takes a cubemap texture which is used to calculate reflections for reflective objects.
shaders/skybox.glsl              - Renders the skybox texture.

BUILDING AND RUNNING
--------------------
The program can be compiled on UNIX-line systems (OS X, Linux, etc.) with the following command:

    c++ `pkg-config --cflags glfw3` -o clangers *.cpp `pkg-config --static --libs glfw3 glew`

The program can then be executed by running the `clangers` binary.

HOW THE PROGRAM WORKS
---------------------
Meshes are stored in an efficient format that is loaded directly into a vertex buffer/index buffer pair at runtime by the mesh class.
Textures are stored as bitmaps and loaded at runtime using the load_texture function in main.cpp
Diffuse textures are 3 channel textures that are used to calculate the diffuse colour of a texture.
Specular textures are 4 channel textures where the red, green and blue channels determine the specular colour of the object, and the alpha channel determines the specular exponent, and the reflectivity of the surface.
