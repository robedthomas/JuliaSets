/**
@file HelperFunctions.h
@author Rob Thomas
@brief Contains functions for ingesting input and processing the Julia set.
*/

#ifndef HELPERFUNCTIONS_H
#define HELPERFUNCTIONS_H

#include <complex.h>
#include <SDL2/SDL.h>


/**
@def GET_ARGS_SUCCEED
@brief Error code indicating that getArgs() succeeded in acquiring all the
necessary command line arguments.
*/
#define GET_ARGS_SUCCEED 0

/**
@def INSUFFICIENT_ARGS_FAIL
@brief Error code indicating that insufficient command line arguments were
supplied by the user.
*/
#define INSUFFICIENT_ARGS_FAIL 2

/**
@def ARG_BELOW_ONE_FAIL
@brief Error code indicating that the user input a non-positive number for the
dimensions or number of threads.
*/
#define ARG_BELOW_ONE_FAIL 3

/**
@def ARG_NOT_A_NUMBER_FAIL
@brief Error code indicating that a non-number value was input by the user for 
any one of the command line arguments.
*/
#define ARG_NOT_A_NUMBER_FAIL 4


/**
@typedef ThreadData
@brief The ThreadData struct contains data members necessary for implementing
multithreading in the Julia set problem. It is used for transmitting data to 
newly created threads.
*/
typedef struct ThreadData
{
	double centerX, centerY, planeWidth, planeHeight;
	double complex C;
	long windowWidth, windowHeight;
	int threadID, numberOfThreads, numIterations;
	SDL_Color ***colorMapPtr;
} ThreadData;

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
			 double *centerY, double complex *C, long *numberOfThreads);

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
double XTransform (int x, double centerX, double planeWidth, long windowWidth);

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
double YTransform (int y, double centerY, double planeHeight, long windowHeight);

/**
@fn waitForClose
@brief Waits until the user closes the window.
@return 0 if the wait is successfully completed, 1 otherwise.
*/
int waitForClose ();

#endif /* HELPERFUNCTIONS_H */