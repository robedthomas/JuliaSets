/**
@file JuliaSet.h
@author Rob Thomas
@brief Contains functions for evaluating the Julia set members in a slice of
the complex plane.
*/

#ifndef JULIASET_H
#define JULIASET_H

#include <complex.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

#include "HelperFunctions.h"

#include "Drawing.h"

/**
@fn f
@brief Applies the function of the form f(z) = z^2 + C to the point Z in the
complex plane.
@param Z The point in the complex plane to apply the function to.
@param C The complex constant defining f(z) = z^2 + C.
@return The result of f(Z).
*/
double complex f (double complex Z, double complex C);

/**
@fn distanceFromOrigin
@brief Computes the distance of a complex point Z from the origin.
@param Z The point in the complex plane whose distance from the origin will be
calculated.
@return The distance of Z from the origin.
*/
double distanceFromOrigin (double complex Z);

/**
@fn partialFill
@brief Partially fills the Julia set depending on the number of threads and 
which thread is currently running.
@param data A void pointer to be cast into a ThreadData struct.
*/
int partialFill (void *data);

/**
@fn isInJuliaSet
@brief Returns whether or not a point in the complex plane is in the Julia set
described by f(z) = z^2 + c.
@param Z The point in the complex plane to check for membership in the Julia set.
@param C A complex constant that helps to define the function f(z).
@param numIterations The number of iterations to apply to point Z.
@param stageEliminated A buffer to which the number of iterations done before
Z could be eliminated will be written. 
@return True if Z is in the Julia set. False otherwise.
*/
bool isInJuliaSet (double complex Z, double complex C, int numIterations,
	    		   int * stageEliminated);

/**
@fn fillJuliaSet
@brief Evaluates each complex point in the window to see if it is in the
Julia set. Colors points appropriately.
@param centerX The X coordinate (in the complex place) of the center of the 
window.
@param centerY The Y coordinate (in the complex place) of the center of the 
window.
@param planeWidth The width (in units) of the slice of the complex plane to be
displayed in the window.
@param planeHeight The height (in units) of the slice of the complex plane to be
displayed in the window.
@param windowWidth The width of the window in pixels.
@param windowHeight The height of the window in pixels.
@param numIterations The number of iterations to be applied to each point while
checking if it is in the Julia set.
@param colorMap An empty 2-dimensional array of colors of size 
windowWidth x windowHeight that indicates the color of each pixel in the window.
@param C The complex constant defining the function f(z) = z^2 + C.
@param numberOfThreads The number of threads that work is split between.
@param threadID The integer value indicating which thread is working right now.
*/
void fillJuliaSet (double centerX, double centerY, double planeWidth, 
				   double planeHeight, long windowWidth, long windowHeight, 
				   int numIterations, SDL_Color **colorMap, double complex C,
				   int numberOfThreads, int threadID);

#endif /* JULIASET_H */

