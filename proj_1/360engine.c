/*
    360engine.c
    Compiled to give a binary version of out graphics engine.
    Written by Josh Davis for Carleton College's CS311 - Computer Graphics.
*/


/*
On macOS, compile with...
	clang -c 360engine.c
...then link with a main program by for example...
	clang main.c 040pixel.o 360engine.o -lglfw -framework OpenGL -framework Cocoa -framework IOKit

On Ubuntu, compile with...
    cc -c 360engine.c
...and then link with a main program by for example...
    cc main.c 040pixel.o 360engine.o -lglfw -lGL -lm -ldl
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <GLFW/glfw3.h>

#include "040pixel.h"

#include "250vector.c"
#include "280matrix.c"
#include "150texture.c"
#include "260shading.c"
#include "260depth.c"
#include "270triangle.c"
#include "350mesh.c"
#include "190mesh2D.c"
#include "250mesh3D.c"
#include "300isometry.c"
#include "300camera.c"
#include "340landscape.c"