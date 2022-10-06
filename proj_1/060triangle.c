/*
    060triangle.c
    C file to rasterize a given triangle and render it on the screen. 
    Written by Cole Weinstein for Carleton College's CS311 - Computer Graphics, taught by Josh Davis.
*/

/* Include statements go here. */
#include <stdio.h>
#include <math.h>
#include <GLFW/glfw3.h>
#include "040pixel.h"

/* Given a triangle and knowledge of its left-most vertex, rasterizes the triangle and renders it. */
void triRenderHelper(double a0, double a1, double b0, double b1, double c0, double c1, double r, double g, double b) {
    int x0 = (int) ceil(a0);
    int x1;

    // now, we render the triangle. the first three cases deal with triangles which have
    // vertical sides and thus only require one horizontally iteratting loop. the vertically
    // iterating loop starts at the ceiling of the bottom edge, determined by the order of the
    // vertices, then runs until the floor of the top edge.
    
    // if a0 == c0, then the left edge (side ac) is vertical and we don't have to worry about rendering it.
    // since a -> b -> c is clockwise, we know a1 < c1.
    if (a0 == c0) {
        while (x0 <= (int)floor(b0)){
            x1 = (int)ceil(a1 + (b1-a1)/(b0-a0)*(x0-a0));
            while (x1 <= (int)floor(c1 + (b1-c1)/(b0-c0)*(x0-c0))) {
                pixSetRGB(x0, x1, r, g, b);
                x1 = x1 + 1;
            }
            x0 = x0 + 1;
        }
    }
    // o.w., if a0 == b0, then the left edge (side ab) is vertical and we don't have to worry about rendering it.
    // sice a -> b -> c is clockwise, we know that a1 > b1.
    else if (a0 == b0) {
        while (x0 <= (int)floor(c0)){
            x1 = (int)ceil(b1 + (c1-b1)/(c0-b0)*(x0-b0));
            while (x1 <= (int)floor(a1 + (c1-a1)/(c0-a0)*(x0-a0))) {
                pixSetRGB(x0, x1, r, g, b);
                x1 = x1 + 1;
            }
            x0 = x0 + 1;
        }
    }
    // o.w., if b0 == c0, then the right edge (side bc) is vertical and we don't have to worry about rendering it.
    // (can't be left edge this time since a known to be left-most vertex.)
    // sice a -> b -> c is clockwise, we know that c1 > b1.
    else if (b0 == c0) {
        while (x0 <= (int)floor(c0)){
            x1 = (int)ceil(a1 + (b1-a1)/(b0-a0)*(x0-a0));
            while (x1 <= (int)floor(a1 + (c1-a1)/(c0-a0)*(x0-a0))) {
                pixSetRGB(x0, x1, r, g, b);
                x1 = x1 + 1;
            }
            x0 = x0 + 1;
        }
    }
    
    // the next two cases deal with triangles which have no vertical sides and thus require
    // two horizontally iterating loops. in summary, these loops together iterate over the
    // edge of the triangle covering the largest horizontal distance, and individually iterate
    // over the other two edges of the triangle, again, determined by the order of the vertices.
    
    // if b0 < c0, then loop from a0 to c0, reframing checks around b0.
    else if (b0 < c0) {
        while (x0 <= (int)floor(b0)){
            x1 = (int)ceil(a1 + (a1-b1)/(a0-b0)*(x0-a0));
            while (x1 <= (int)floor(a1 + (c1-a1)/(c0-a0)*(x0-a0))) {
                pixSetRGB(x0, x1, r, g, b);
                x1 = x1 + 1;
            }
            x0 = x0 + 1;
        }
        while (x0 <= (int)floor(c0)){
            x1 = (int)ceil(c1 + (c1-b1)/(c0-b0)*(x0-c0));
            while (x1 <= (int)floor(a1 + (c1-a1)/(c0-a0)*(x0-a0))) {
                pixSetRGB(x0, x1, r, g, b);
                x1 = x1 + 1;
            }
            x0 = x0 + 1;
        }
    }
    // o.w., c0 < b0, so loop from a0 to b0 and reframe checks around c0.
    else {
        while (x0 <= (int)floor(c0)){
            x1 = (int)ceil(a1 + (b1-a1)/(b0-a0)*(x0-a0));
            while (x1 <= (int)floor(a1 + (c1-a1)/(c0-a0)*(x0-a0))) {
                pixSetRGB(x0, x1, r, g, b);
                x1 = x1 + 1;
            }
            x0 = x0 + 1;
        }
        while (x0 <= (int)floor(b0)){
            x1 = (int)ceil(a1 + (b1-a1)/(b0-a0)*(x0-a0));
            while (x1 <= (int)floor(b1 + (c1-b1)/(c0-b0)*(x0-b0))) {
                pixSetRGB(x0, x1, r, g, b);
                x1 = x1 + 1;
            }
            x0 = x0 + 1;
        }
    }
}

/* Determines left-most vertex of a triangle and calls triRenderHelper() to render the triangle. */
void triRender(double a0, double a1, double b0, double b1, double c0, double c1, double r, double g, double b) {
    if (a0 <= b0 && a0 <= c0)
        triRenderHelper(a0, a1, b0, b1, c0, c1, r, g, b);
    else if(b0 <= a0 && b0 <= c0)
        triRenderHelper(b0, b1, c0, c1, a0, a1, r, g, b);
    else
        triRenderHelper(c0, c1, a0, a1, b0, b1, r, g, b);
}