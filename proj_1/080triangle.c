/*
    080triangle.c
    C file to rasterize a given triangle and render it on the screen (this time using arrays instead of multiple parameters).
    Written by Cole Weinstein for Carleton College's CS311 - Computer Graphics, taught by Josh Davis.
*/

/* Include statements go here. */
#include <stdio.h>
#include <math.h>
#include <GLFW/glfw3.h>
#include "040pixel.h"

/* Given a triangle and knowledge of its left-most vertex, rasterizes the triangle and renders it. */
void triRenderHelper(const double a[2], const double b[2], const double c[2], const double rgb[3]) {
    int x[2];
    x[0] = (int)ceil(a[0]);

    // now, we render the triangle. the first three cases deal with triangles which have
    // vertical sides and thus only require one horizontally iteratting loop. the vertically
    // iterating loop starts at the ceiling of the bottom edge, determined by the order of the
    // vertices, then runs until the floor of the top edge.
    
    // if a[0] == c[0], then the left edge (side ac) is vertical and we don't have to worry about rendering it.
    // since a -> b -> c is clockwise, we know a[1] < c[1].
    if (a[0] == c[0]) {
        while (x[0] <= (int)floor(b[0])){
            x[1] = (int)ceil(a[1] + (b[1]-a[1])/(b[0]-a[0])*(x[0]-a[0]));
            while (x[1] <= (int)floor(c[1] + (b[1]-c[1])/(b[0]-c[0])*(x[0]-c[0]))) {
                pixSetRGB(x[0], x[1], rgb[0], rgb[1], rgb[2]);
                x[1] = x[1] + 1;
            }
            x[0] = x[0] + 1;
        }
    }
    // o.w., if a[0] == b[0], then the left edge (side ab) is vertical and we don't have to worry about rendering it.
    // sice a -> b -> c is clockwise, we know that a[1] > b[1].
    else if (a[0] == b[0]) {
        while (x[0] <= (int)floor(c[0])){
            x[1] = (int)ceil(b[1] + (c[1]-b[1])/(c[0]-b[0])*(x[0]-b[0]));
            while (x[1] <= (int)floor(a[1] + (c[1]-a[1])/(c[0]-a[0])*(x[0]-a[0]))) {
                pixSetRGB(x[0], x[1], rgb[0], rgb[1], rgb[2]);
                x[1] = x[1] + 1;
            }
            x[0] = x[0] + 1;
        }
    }
    // o.w., if b[0] == c[0], then the right edge (side bc) is vertical and we don't have to worry about rendering it.
    // (can't be left edge this time since a known to be left-most vertex.)
    // sice a -> b -> c is clockwise, we know that c[1] > b[1].
    else if (b[0] == c[0]) {
        while (x[0] <= (int)floor(c[0])){
            x[1] = (int)ceil(a[1] + (b[1]-a[1])/(b[0]-a[0])*(x[0]-a[0]));
            while (x[1] <= (int)floor(a[1] + (c[1]-a[1])/(c[0]-a[0])*(x[0]-a[0]))) {
                pixSetRGB(x[0], x[1], rgb[0], rgb[1], rgb[2]);
                x[1] = x[1] + 1;
            }
            x[0] = x[0] + 1;
        }
    }
    
    // the next two cases deal with triangles which have no vertical sides and thus require
    // two horizontally iterating loops. in summary, these loops together iterate over the
    // edge of the triangle covering the largest horizontal distance, and individually iterate
    // over the other two edges of the triangle, again, determined by the order of the vertices.
    
    // if b[0] < c[0], then loop from a[0] to c[0], reframing checks around b[0].
    else if (b[0] < c[0]) {
        while (x[0] <= (int)floor(b[0])){
            x[1] = (int)ceil(a[1] + (a[1]-b[1])/(a[0]-b[0])*(x[0]-a[0]));
            while (x[1] <= (int)floor(a[1] + (c[1]-a[1])/(c[0]-a[0])*(x[0]-a[0]))) {
                pixSetRGB(x[0], x[1], rgb[0], rgb[1], rgb[2]);
                x[1] = x[1] + 1;
            }
            x[0] = x[0] + 1;
        }
        while (x[0] <= (int)floor(c[0])){
            x[1] = (int)ceil(c[1] + (c[1]-b[1])/(c[0]-b[0])*(x[0]-c[0]));
            while (x[1] <= (int)floor(a[1] + (c[1]-a[1])/(c[0]-a[0])*(x[0]-a[0]))) {
                pixSetRGB(x[0], x[1], rgb[0], rgb[1], rgb[2]);
                x[1] = x[1] + 1;
            }
            x[0] = x[0] + 1;
        }
    }
    // o.w., c[0] < b[0], so loop from a[0] to b[0] and reframe checks around c[0].
    else {
        while (x[0] <= (int)floor(c[0])){
            x[1] = (int)ceil(a[1] + (b[1]-a[1])/(b[0]-a[0])*(x[0]-a[0]));
            while (x[1] <= (int)floor(a[1] + (c[1]-a[1])/(c[0]-a[0])*(x[0]-a[0]))) {
                pixSetRGB(x[0], x[1], rgb[0], rgb[1], rgb[2]);
                x[1] = x[1] + 1;
            }
            x[0] = x[0] + 1;
        }
        while (x[0] <= (int)floor(b[0])){
            x[1] = (int)ceil(a[1] + (b[1]-a[1])/(b[0]-a[0])*(x[0]-a[0]));
            while (x[1] <= (int)floor(b[1] + (c[1]-b[1])/(c[0]-b[0])*(x[0]-b[0]))) {
                pixSetRGB(x[0], x[1], rgb[0], rgb[1], rgb[2]);
                x[1] = x[1] + 1;
            }
            x[0] = x[0] + 1;
        }
    }
}

/* Determines left-most vertex of a triangle and calls triRenderHelper() to render the triangle. */
void triRender(const double a[2], const double b[2], const double c[2], const double rgb[3]) {
    if (a[0] <= b[0] && a[0] <= c[0])
        triRenderHelper(a, b, c, rgb);
    else if(b[0] <= a[0] && b[0] <= c[0])
        triRenderHelper(b, c, a, rgb);
    else
        triRenderHelper(c, a, b, rgb);
}