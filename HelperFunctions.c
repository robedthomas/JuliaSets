/**
@file HelperFunctions.c
@author Rob Thomas
@brief Contains functions for ingesting input and processing the Julia set.
*/


#include <complex.h>
#include <stdlib.h>
#include <SDL2/SDL.h>

#include "HelperFunctions.h"


/**
@fn getArgs
@brief Ingests the command line arguments provided by the user.
@param argc The number of command line arguments passed in.
@param argv The list of command line arguments (list of strings).
@param windowWidth Pointer to where window width will be stored.
@param windowHeight Pointer to where window height will be stored.
@param planeWidth Pointer to where plane width will be stored.
@param planeHeight Pointer to where plane height will be stored.
@param centerX Pointer to where centerX will be stored.
@param centerY Pointer to where centerY will be stored.
@param C Pointer to where the complex constant c will be stored.
@param numberOfThreads Pointer to where the number of threads will be stored.
@return An error code. 0 if operation was successful. 
*/
int getArgs (int argc, char *argv[], long *windowWidth, long *windowHeight, 
			 double *planeWidth, double *planeHeight, double *centerX, 
			 double *centerY, double complex *C, long *numberOfThreads)
{
	char e = '\0';
	char *endptr = &e;

	/*** Check for too few arguments. ***/
	if (argc < 10)
	{
		fprintf(stderr, "Insufficient arguments given.\n");

		return INSUFFICIENT_ARGS_FAIL;
	}

	/*** Read in and convert arguments as necessary. ***/
	*windowWidth = strtol(argv[1], &endptr, 10);
	*windowHeight = strtol(argv[2], &endptr, 10);
	*planeWidth = strtod(argv[3], &endptr);
	*planeHeight = strtod(argv[4], &endptr);
	*centerX = strtod(argv[5], &endptr);
	*centerY = strtod(argv[6], &endptr);
	double a = strtod(argv[7], &endptr);
	double b = strtod(argv[8], &endptr);
	*numberOfThreads = strtol(argv[9], &endptr, 10);

	/*** Check for non-numbers being input by the user. ***/
	if ( (*windowWidth == 0 && argv[1][0] != '0')    || 
		 (*windowHeight == 0 && argv[2][0] != '0')   ||
		 (*planeWidth == 0.0 && argv[3][0] != '0')   ||
		 (*planeHeight == 0.0 && argv[4][0] != '0')  ||
		 (*centerX == 0 && argv[5][0] != '0')		 ||
		 (*centerY == 0 && argv[6][0] != '0')		 ||
		 (a == 0 && argv[7][0] != '0')				 ||
		 (b == 0 && argv[8][0] != '0')				 ||
		 (*numberOfThreads == 0 && argv[9][0] != '0')  )
	{
		fprintf(stderr, "Non-number arg given. All args must be numbers.\n");

		return ARG_NOT_A_NUMBER_FAIL;
	}

	/*** Check for non-positive values of window dimension, plane dimensions, or
		 the number of threads. ***/
	if (*windowWidth <= 0 || *windowHeight <= 0)
	{
		fprintf(stderr, "Window dimensions (args 1 and 2) must be greater than 0.\n");

		return ARG_BELOW_ONE_FAIL;
	}
	else if (*planeWidth <= 0.0 || *planeHeight <= 0.0)
	{
		fprintf(stderr, "Plane dimensions (args 3 and 4) must be greater than 0.\n");

		return ARG_BELOW_ONE_FAIL;
	}
	else if (*numberOfThreads <= 0)
	{
		fprintf(stderr, "Number of threads (arg 9) must be greater than 0.\n");

		return ARG_BELOW_ONE_FAIL;
	}


	/* @DEBUG: Print out the values after they've been read in. */
	/* printf("W: %ld, H: %ld, Wp: %f, Hp: %f\nCx: %f, Cy: %f, a: %f, b: %f\nnumThreads: %ld\n",
		   *windowWidth, *windowHeight, *planeWidth, *planeHeight, 
		   *centerX, *centerY, a, b, *numberOfThreads); */

	/*** Set the complex constant C based on the a and b provided by the user. ***/
	*C = a + b * I;

	return GET_ARGS_SUCCEED;
}

/**
@fn XTransform
@brief Converts window coordinates (x = 0 is left of window) into complex
plane coordinates.
@param x The x coordinate (in pixels) to be converted.
@param centerX The x coordinate in the complex plane that the window will
center on.
@param planeWidth The width (in units) of the slice of the complex plane that
the window will display.
@param windowWidth The width (in pixels) of the window.
@return The complex X coordinate corresponding to x.
*/
double XTransform (int x, double centerX, double planeWidth, long windowWidth)
{
	/* Formula: T(x) = Wp((x / W) - 0.5) + Cx */

	return (planeWidth * ( ((double)x / (double)windowWidth) - 0.5 ) + centerX);
}

/**
@fn YTransform
@brief Converts window coordinates (y = 0 is top of window) into complex
plane coordinates.
@param y The y coordinate (in pixels) to be converted.
@param centerY The y coordinate in the complex plane that the window will
center on.
@param planeHeight The height (in units) of the slice of the complex plane that
the window will display.
@param windowHeight The height (in pixels) of the window.
@return The complex Y coordinate corresponding to y.
*/
double YTransform (int y, double centerY, double planeHeight, long windowHeight)
{
	/* Formula: T(y) = Hp(0.5 - (y / H)) + Cy */

	return (planeHeight * ( 0.5 - ((double)y / (double)windowHeight) ) + centerY);
}

/**
@fn waitForClose
@brief Waits until the user closes the window.
@return 0 if the wait is successfully completed, 1 otherwise.
*/
int waitForClose ()
{
	SDL_Event event;

	do
	{
		if (SDL_WaitEvent(&event) == 0)
		{
			return 1;
		}

	} while (event.type != SDL_QUIT);

	return 0;
}