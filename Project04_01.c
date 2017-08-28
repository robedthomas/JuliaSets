/**
@file Project04_01.c
@author Rob Thomas
@brief Contains the main function for displaying an arbitrary section of a
Julia set.
*/


#include <complex.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_thread.h>

#include "JuliaSet.h"
#include "Drawing.h"
#include "HelperFunctions.h"

/**
@def NUM_ITERATIONS
@brief The number of iterations performed on each point to determine if it is in
the Julia set.
*/
#define NUM_ITERATIONS 100

/**
@def SUCCESS
@brief The error code for a successful operation.
*/
#define SUCCESS 0

/**
@def FAILURE
@brief The error code for a failed operation (due to error while waiting for
user to close the window).
*/
#define FAILURE 1

/**
@fn main
@brief Generates an image of a Julia set with the properties given by the user.
@details main() requires nine command line arguments in the following order:
	windowWidth: the width (in pixels) of the window in which the Julia set will 
				 be displayed (a positive integer)
	windowHeight: the height (in pixels) of the window in which the Julia set will 
				  be displayed (a positive integer)
	planeWidth: the width (on the real number line) of the complex plane to be examined
				(a floating point number)
	planeHeight: the height (on the real number line) of the complex plane to be examined
				 (a floating point number)
	centerX: the value on the X axis of the complex plane which the image of the 
			 Julia set will be centered on (a floating point number)
	centerY: the value on the Y axis of the complex plane which the image of the 
			 Julia set will be centered on (a floating point number)
	a: the real component of the complex constant C, which is a property that
	   characterizes each Julia set (a floating point number)
	b: the imaginary component of the complex constant C (a floating point number)
	numberOfThreads: the number of threads to be used to calculate Julia set
					 (a positive integer)
*/
int main (int argc, char *argv[])
{
	long windowWidth, windowHeight, numberOfThreads;
	double planeWidth, planeHeight, centerX, centerY;
	double complex C;
	/*** Read in command line arguments. ***/
	int result = getArgs(argc, argv, &windowWidth, &windowHeight, &planeWidth,
						 &planeHeight, &centerX, &centerY, &C, &numberOfThreads);
	/*** If there are any invalid args, exit immediately. getArgs() will have
		 reported the exact problem to the user. ***/
	if (result)
	{
		exit(result);
	}


	/*** Calculate the color map through determining the Julia set. ***/
	SDL_Color **colorMap = newColorMap(windowWidth, windowHeight);

	SDL_Thread *threadList[numberOfThreads];
	ThreadData dataList[numberOfThreads];

	/* Define the data packets that will be passed to each new thread.
	   Do this before starting the timer so that it doesn't influence the 
	   measured processing time. */
	for (int threadID = 0; threadID < numberOfThreads; threadID++)
	{
		
		dataList[threadID].centerX = centerX;
		dataList[threadID].centerY = centerY;
		dataList[threadID].planeWidth = planeWidth;
		dataList[threadID].planeHeight = planeHeight;
		dataList[threadID].C = C;
		dataList[threadID].windowWidth = windowWidth;
		dataList[threadID].windowHeight = windowHeight;
		dataList[threadID].threadID = threadID;
		dataList[threadID].numberOfThreads = (int)numberOfThreads;
		dataList[threadID].numIterations = NUM_ITERATIONS;
		dataList[threadID].colorMapPtr = &colorMap;
	}

	Uint32 startTime = SDL_GetTicks();

		/* Create the number of threads specified by the user, and divide the 
		   work up so that each thread fills in the same number of columns in 
		   the color map. */
	for (int threadID = 0; threadID < numberOfThreads; threadID++)
	{
		/* Make a new thread and pass it the appropriate data packet. */
		threadList[threadID] = SDL_CreateThread( partialFill, "Current Thread",
												(void*)&(dataList[threadID]) );
	}

		/* Wait for each thread to finish before drawing the Julia set. */
	for (int threadID = 0; threadID < numberOfThreads; threadID++)
	{
		SDL_WaitThread(threadList[threadID], NULL);
	}

	Uint32 endTime = SDL_GetTicks();

	/*** Print out how long processing took with the given number of threads. ***/
	printf("Processing time: %dms\n", endTime - startTime);


	/*** Initialize SDL. ***/
	SDL_Window *window = NULL;
	SDL_Renderer *renderer = NULL;	

	if ( !initializeSDL("Julia Set", &window, &renderer, windowWidth, 
						windowHeight) )
	{
		fprintf(stderr, "SDL failed to initialize.\n");

		exit(FAILURE);
	}

	/*** Print the color map to the window. ***/
	drawJuliaSet(colorMap, renderer, windowWidth, windowHeight);

	/*** Wait for the user to close the window, then clean up SDL and exit. ***/ 
	result = waitForClose();

	if (result)
	{
		fprintf(stderr, "Error while waiting for user to close window.\n");

		cleanAndExit(window, renderer, colorMap, windowWidth, windowHeight,
					 FAILURE);
	}
	else
	{
		cleanAndExit(window, renderer, colorMap, windowWidth, windowHeight,
			         SUCCESS);
	}
}