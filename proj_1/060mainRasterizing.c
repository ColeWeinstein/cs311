/*
    060mainRasterizing.c
    C file to invoke 060triangle.c and render a specified triangle. 
    Written by Cole Weinstein for Carleton College's CS311 - Computer Graphics, taught by Josh Davis.
    Adapted from Josh Davis's 040mainLinking.c.
*/

#include <stdio.h>
#include <GLFW/glfw3.h>
#include "040pixel.h"
#include "060triangle.c"

int main(void) {
    /* Make a 512 x 512 window with the title 'Rasterizer'. This function 
    returns 0 if no error occurred. */
    if (pixInitialize(512, 512, "Rasterizer") != 0)
        return 1;
    /* Clear the window to black. */
    pixClearRGB(0.0, 0.0, 0.0);
    
    // demonstration of rasterizer working.
    triRender(200.1, 100.1, 50.1, 50.1, 400.1, 400.1, 1, .5, 0);

    /* Run the event loop. The callbacks that were registered above are 
    invoked as needed. */
    pixRun();
    /* In this course, whenever you call an 'initialize' function to set up some 
    resources, you must later call a matching 'finalize' function to clean up 
    those resources. */
    pixFinalize();
    return 0;
}

