/*
    080mainRasterizing.c
    C file to invoke 080triangle.c and render a specified triangle. 
    Written by Cole Weinstein for Carleton College's CS311 - Computer Graphics, taught by Josh Davis.
    Adapted from Josh Davis's 040mainLinking.c.
*/

/* On macOS, compile with...
    clang 080mainRasterizing.c 040pixel.o -lglfw -framework OpenGL -framework Cocoa -framework IOKit
On Ubuntu, compile with...
    cc 080mainRasterizing.c 040pixel.o -lglfw -lGL -lm -ldl
*/

#include <stdio.h>
#include <GLFW/glfw3.h>
#include "040pixel.h"
#include "080triangle.c"
#include "080vector.c"

int main(void) {
    /* Make a 512 x 512 window with the title 'Rasterizer'. This function 
    returns 0 if no error occurred. */
    if (pixInitialize(512, 512, "Rasterizer") != 0)
        return 1;
    /* Clear the window to black. */
    pixClearRGB(0.0, 0.0, 0.0);
    
    // demonstration of rasterizer working.
    double a[2], b[2], c[2], rgb[3];
    a[0] = 450, a[1] = 250;
    b[0] = 50, b[1] = 250;
    c[0] = 250, c[1] = 50;
    rgb[0] = 1, rgb[1] = .5, rgb[2] = 0;
    triRender(a, b, c, rgb);

    /* Run the event loop. The callbacks that were registered above are 
    invoked as needed. */
    pixRun();
    /* In this course, whenever you call an 'initialize' function to set up some 
    resources, you must later call a matching 'finalize' function to clean up 
    those resources. */
    pixFinalize();
    return 0;
}