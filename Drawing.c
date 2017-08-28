/**
@file Drawing.c
@author Rob Thomas
@brief Contains functions for drawing a Julia set as well as coloring the 
Julia set.
*/

#include <SDL2/SDL.h>
#include <stdbool.h>

#include "HelperFunctions.h"

#include "Drawing.h"


/**
@fn initializeSDL
@brief Initializes SDL to allow the use of its functions and features.
@param title The string title to name the window.
@param window Pointer to an uninitialized SDL_Window struct.
@param renderer Pointer to an uninitialized SDL_Renderer struct.
*/
bool initializeSDL (char *title, SDL_Window **window, SDL_Renderer **renderer,
					long windowWidth, long windowHeight)
{
	if (title == NULL)
	{
		return false;
	}
	if (window == NULL)
	{
		return false;
	}
	if (renderer == NULL)
	{
		return false;
	}

	/* Initialize SDL with video system. */
	if( SDL_Init(SDL_INIT_VIDEO) ) 
	{
		return false;
	}

	/* Create a window centered in the screen. */
	if (!( *window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, 
		                              SDL_WINDOWPOS_CENTERED, (int)windowWidth, 
		                              (int)windowHeight, SDL_WINDOW_SHOWN) ))
	{
		return false;
	}

	/* Create a renderer for the window with VSYNC and hardware acceleration
	   enabled. */
	if (!( *renderer = SDL_CreateRenderer(*window, -1, 
												SDL_RENDERER_ACCELERATED | 
												SDL_RENDERER_PRESENTVSYNC) ))
	{
		return false;
	}


	return true;
}

/**
@fn cleanAndExit
@brief Cleans up SDL's subsystems and exits the program.
@param window Pointer to the window used for displaying.
@param renderer Pointer to the renderer used for rendering.
@param colorMap A 2D array of SDL_Color structs that was dynamically allocated.
@param windowWidth The width (in pixels) of the window.
@param windowHeight The height (in pixels) of the window.
@oaram errorCode The code to pass to exit().
*/
void cleanAndExit(SDL_Window *window, SDL_Renderer *renderer, 
				  SDL_Color ** colorMap, long windowWidth, long windowHeight,
				  int errorCode)
{
	/* Free the window and renderer. */
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	/* Free the color map. */
	freeColorMap(colorMap, windowWidth, windowHeight);

	/* Exit SDL. */
	if(SDL_WasInit(0))
	{
		SDL_Quit();
	}

	/* Exit with the given code. */
	exit(errorCode);
}

/**
@fn newColorMap
@brief Dynamically allocates a windowWidth x windowHeight 2D array of colors.
@param windowWidth The width (in pixels) of the window.
@param windowHeight The height (in pixels) of the window.
@return A dynamically allocated 2D array of SDL_Color structs.
*/
SDL_Color ** newColorMap (long windowWidth, long windowHeight)
{
	SDL_Color ** colorMap = (SDL_Color**)malloc(sizeof(SDL_Color*) * windowWidth);

	for (int i = 0; i < windowWidth; i++)
	{
		colorMap[i] = (SDL_Color*)malloc(sizeof(SDL_Color) * windowHeight);
	}

	return colorMap;
}

/**
@fn freeColorMap
@brief Frees a dynamically allocated color map.
@param colorMap The color map to be freed.
@param windowWidth The width of the color map.
@param windowHeight The height of the color map.
*/
void freeColorMap (SDL_Color ** colorMap, long windowWidth, long windowHeight)
{

	for (int i = 0; i < windowWidth; i++)
	{
		free(colorMap[i]);
	}

	free(colorMap);
}

/**
@fn drawJuliaSet
@brief Draws the Julia set described by colorMap.
@param colorMap The 2D array of colors describing each pixel in the window.
@param window Pointer to the renderer to draw with.
@param windowWidth The width of the window (in pixels).
@param windowHeight The height of the window (in pixels).
*/
void drawJuliaSet (SDL_Color **colorMap, SDL_Renderer *renderer, long windowWidth,
				   long windowHeight)
{
	/* Clear the window to black first. */
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);

	/* Draw each pixel as specified by the color map. */
	SDL_Rect currentPixel;
	currentPixel.w = 2;
	currentPixel.h = 2;

	for (int x = 0; x < windowWidth; x++)
	{
		for (int y = 0; y < windowHeight; y++)
		{
			/* Set the draw color to the color from the color map. */
			SDL_SetRenderDrawColor(renderer, colorMap[x][y].r, colorMap[x][y].g,
								   colorMap[x][y].b, colorMap[x][y].a);
			/* Draw the current pixel with the appropriate color. */
			currentPixel.x = x;
			currentPixel.y = y;
			
			SDL_RenderDrawRect(renderer, &currentPixel);
		}
	}

	/* Present the drawn set. */
	SDL_RenderPresent(renderer);
}

/**
@fn colorInSet
@brief Gives the color of points that are in the Julia set.
@return An SDL_Color struct representing the color of points in the Julia set.
*/
SDL_Color colorInSet ()
{
	SDL_Color color;

	color.r = RED_IN_SET;
	color.g = GREEN_IN_SET;
	color.b = BLUE_IN_SET;
	color.a = OPACITY_IN_SET;

	return color;
}

/**
@fn colorOutOfSet
@brief Gives the color of points that are outside of the Julia set.
@param stageEliminated The number of iterations completed before the point was
eliminated from the Julia set.
@return An SDL_Color struct representing the color of points outside of the 
Julia set.
*/
SDL_Color colorOutOfSet (int stageEliminated)
{
	SDL_Color color;

	color.r = RED_OUTOF_SET + (RED_DELTA * stageEliminated);
	color.g = GREEN_OUTOF_SET + (GREEN_DELTA * stageEliminated);
	color.b = BLUE_OUTOF_SET + (BLUE_DELTA * stageEliminated);
	color.a = OPACITY_OUTOF_SET + (OPACITY_DELTA * stageEliminated);

	return color;
}