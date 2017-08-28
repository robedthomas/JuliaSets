/**
@file JuliaSet.c
@author Rob Thomas
@brief Contains functions for evaluating the Julia set members in a slice of
the complex plane.
*/


#include <complex.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <math.h>

#include "Drawing.h"
#include "HelperFunctions.h"

#include "JuliaSet.h"


/**
@fn f
@brief Applies the function of the form f(z) = z^2 + C to the point Z in the
complex plane.
@param Z The point in the complex plane to apply the function to.
@param C The complex constant defining f(z) = z^2 + C.
@return The result of f(Z).
*/
double complex f (double complex Z, double complex C)
{
	return ((Z * Z) + C);
}

/**
@fn distanceFromOrigin
@brief Computes the distance of a complex point Z from the origin.
@param Z The point in the complex plane whose distance from the origin will be
calculated.
@return The distance of Z from the origin.
*/
double distanceFromOrigin (double complex Z)
{
	return sqrt( (creal(Z) * creal(Z)) + (cimag(Z) * cimag(Z)) );
}

/**
@fn partialFill
@brief Partially fills the Julia set depending on the number of threads and 
which thread is currently running.
@param data A void pointer to be cast into a ThreadData struct.
*/
int partialFill (void *data)
{
	/* Retrieve the data passed from SDL_CreateThread. */
	ThreadData *d = (ThreadData*)data;

	/* Call fillJuliaSet with the data passed in. */
	fillJuliaSet (d->centerX, d->centerY, d->planeWidth, 
				   d->planeHeight, d->windowWidth, d->windowHeight, 
				   d->numIterations, *(d->colorMapPtr), d->C,
				   d->numberOfThreads, d->threadID);

	return 0;
}

/**
@fn isInJuliaSet
@brief Returns whether or not a point in the complex plane is in the Julia set
described by f(z) = z^2 + c.
@details This function determines whether or not the point Z in the complex 
plane is in the Julia set described by f(z) = z^2 + C. To do this, the function
f is iterated on Z a finite number of times. If at any point the result is 
greater than 2 units away from the origin, then Z is considered to NOT be in the
Julia set. This function also writes to a buffer the number of iterations 
applied before Z could be eliminated from the set.
@param Z The point in the complex plane to check for membership in the Julia set.
@param C A complex constant that helps to define the function f(z).
@param numIterations The number of iterations to apply to point Z.
@param stageEliminated A buffer to which the number of iterations done before
Z could be eliminated will be written. 
@return True if Z is in the Julia set. False otherwise.
*/
bool isInJuliaSet (double complex Z, double complex C, int numIterations,
	    		   int * stageEliminated)
{
	double complex prevZ = Z;
	double complex currentZ = Z;

	for (int i = 0; i < numIterations; i++)
	{
		/* Apply the function f to Z. */
		currentZ = f(currentZ, C);

		/* Check if Z is now 2 or more units away from the origin. */
		if (distanceFromOrigin(currentZ) > 2.0)
		{
			/* If so, record which iteration this is and return. */
			*stageEliminated = i;

			/* @DEBUG: Notify the user if a point is NOT in the set. */
			//printf("(%f, %f) NOT in the Julia set!\n", creal(Z), cimag(Z));

			return false;
		}

		/* Check if Z stayed the same after an iteration. If so, it will remain
		   the same for all of the remaining iterations. In this case, we know
		   that Z cannot surpass 2 units from the origin and thus must be in
		   the Julia set. */
		if (currentZ == prevZ)
		{
			return true;
		}

		prevZ = currentZ;
	}

	return true;
}

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
				   int numberOfThreads, int threadID)
{
	int stageEliminated = -1;

	/* Fill each column that is numberOfThreads apart, starting at column
	   threadID. */
	for (int x = threadID; x < windowWidth; x += numberOfThreads)
	{
		for (int y = 0; y < windowHeight; y++)
		{
			/* Translate from window coordinates to complex plane coordinates. */
			double compX = XTransform(x, centerX, planeWidth, windowWidth);
			double compY = YTransform(y, centerY, planeHeight, windowHeight);
			/* Define Z based on these coordinates. */
			double complex Z = compX + compY * I;

			/* Check if Z is in the Julia set or not. */
			if (isInJuliaSet( Z, C, numIterations, &stageEliminated ))
			{
				colorMap[x][y] = colorInSet();
			}
			else
			{
				colorMap[x][y] = colorOutOfSet(stageEliminated);
			}
		}
	}
}