/*
    050mainPainting.c
    C file based on 040mainLinking.c which allows the user to paint on the pixel canvas.
    Adapted by Cole Weinstein. Original 040mainLinking.c file written by Josh Davis for Carleton College's CS311 - Computer Graphics
*/

/* On macOS, compile with...
    clang 050mainPainting.c 040pixel.o -lglfw -framework OpenGL -framework Cocoa -framework IOKit
On Ubuntu, compile with...
    cc 050mainPainting.c 040pixel.o -lglfw -lGL -lm -ldl
*/

#include <stdio.h>
#include <math.h>
#include <GLFW/glfw3.h>
#include "040pixel.h"

/* Some global variables for tracking the current paint color and whether the 
user is dragging or just moving the mouse. */
double red = 1.0, green = 1.0, blue = 1.0;
int mouseIsDown = 0;

/* This function is a user interface 'callback'. Once we register it with the 
user interface infrastructure, it is called whenever the user releases a 
keyboard key. For details, see the pixSetKeyUpHandler documentation. */
void handleKeyUp(int key, int shiftIsDown, int controlIsDown,
        int altOptionIsDown, int superCommandIsDown) {
    printf("handleKeyUp: %d, shift %d, control %d, altOpt %d, supComm %d\n",
        key, shiftIsDown, controlIsDown, altOptionIsDown, superCommandIsDown);
    
    // the following lines change the paint color when the appropraite key is pressed.
    // R -> red, G -> green, B-> blue, W -> white
    // C -> cyan, M -> magenta, Y -> yellow, K -> black
    if (key == GLFW_KEY_R){
        red = 1, green = 0, blue = 0;
        printf("Paint set to red!\n");
    }
    if (key == GLFW_KEY_G){
        red = 0, green = 1, blue = 0;
        printf("Paint set to green?\n");
    }
    if (key == GLFW_KEY_B){
        red = 0, green = 0, blue = 1;
        printf("Paint set to blue.\n");
    }
    if (key == GLFW_KEY_C){
        red = 0, green = 1, blue = 1;
        printf("Paint set to cyan :)\n");
    }   
    if (key == GLFW_KEY_M){
        red = 1, green = 0, blue = 1;
        printf("Paint set to magenta ;p\n");
    }
    if (key == GLFW_KEY_Y){
        red = 1, green = 1, blue = 0;
        printf("Paint set to yellow :O\n");
    }
    if (key == GLFW_KEY_W){
        red = 1, green = 1, blue = 1;
        printf("Paint set to white (* _ *)\n");
    }
    if (key == GLFW_KEY_K){
        red = 0, green = 0, blue = 0;
        printf("Paint set to black (* ~ *)\n");
    }
    // user can clear the screen by pressing the "Backspace" key.
    if (key == GLFW_KEY_BACKSPACE){
        pixClearRGB(0, 0, 0);
        printf("Screen cleared.\n");
    }
}

/* Similarly, the following callbacks handle some mouse interactions. */
void handleMouseUp(double x, double y, int button, int shiftIsDown, 
        int controlIsDown, int altOptionIsDown, int superCommandIsDown) {
    // when user stops pressing LMB (left mouse button), track that so we stop painting.
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        mouseIsDown = 0;
    }
    printf("handleMouseUp: %d, shift %d, control %d, altOpt %d, supComm %d\n",
        button, shiftIsDown, controlIsDown, altOptionIsDown, 
        superCommandIsDown);
}

/* Callback to handle more mouse interactions (specifically, mouse button pressed down). */
void handleMouseDown(double x, double y, int button, int shiftIsDown, 
        int controlIsDown, int altOptionIsDown, int superCommandIsDown) {
    // if LMB pressed, track that and start painting.
    if (button == GLFW_MOUSE_BUTTON_LEFT){
        mouseIsDown = 1;
    }
    if (mouseIsDown == 1){
        pixSetRGB(x, y, red, green, blue);
    }
    printf("handleMouseDown: %d, shift %d, control %d, altOpt %d, supComm %d\n",
        button, shiftIsDown, controlIsDown, altOptionIsDown, 
        superCommandIsDown);
}

void handleMouseMove(double x, double y) {
    // while LMB pressed, paint the canvas!
    if (mouseIsDown == 1){
        pixSetRGB(x, y, red, green, blue);
    }
    printf("handleMouseMove: x %f, y %f\n", x, y);
}

void handleMouseScroll(double xOffset, double yOffset) {
    printf("handleMouseScroll: xOffset %f, yOffset %f\n", xOffset, yOffset);
}

/* This callback is called once per animation frame. As parameters it receives 
the time for the current frame and the time for the previous frame. Both times 
are measured in seconds since some distant past time. */ 
void handleTimeStep(double oldTime, double newTime) {
    if (floor(newTime) - floor(oldTime) >= 1.0)
        printf("handleTimeStep: %f frames/s\n", 1.0 / (newTime - oldTime));
}

/* You can also set callbacks for key down, key repeat, and mouse down. See 
040pixel.h for details. */

int main(void) {
    /* Make a 512 x 512 window with the title 'Pixel Graphics'. This function 
    returns 0 if no error occurred. */
    if (pixInitialize(512, 512, "Pixel Graphics") != 0)
        return 1;
    /* Register the callbacks (defined above) with the user interface, so that 
    they are called as needed during pixRun (invoked below). */
    pixSetKeyUpHandler(handleKeyUp);
    pixSetMouseUpHandler(handleMouseUp);
    pixSetMouseDownHandler(handleMouseDown);
    pixSetMouseMoveHandler(handleMouseMove);
    pixSetMouseScrollHandler(handleMouseScroll);
    pixSetTimeStepHandler(handleTimeStep);
    /* Clear the window to black. */
    pixClearRGB(0.0, 0.0, 0.0);
    /* Run the event loop. The callbacks that were registered above are 
    invoked as needed. */
    pixRun();
    /* In this course, whenever you call an 'initialize' function to set up some 
    resources, you must later call a matching 'finalize' function to clean up 
    those resources. */
    pixFinalize();
    return 0;
}

