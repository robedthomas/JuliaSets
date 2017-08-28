/**
@file Drawing.h
@author Rob Thomas
@brief Contains functions for drawing a Julia set as well as coloring the 
Julia set.
*/

#ifndef DRAWING_H
#define DRAWING_H

#include <SDL2/SDL.h>


/**
@def RED_IN_SET
@brief The default value of red for a point in the Julia set.
*/
#define RED_IN_SET 0

/**
@def GREEN_IN_SET
@brief The default value of green for a point in the Julia set.
*/
#define GREEN_IN_SET 0

/**
@def BLUE_IN_SET
@brief The default value of blue for a point in the Julia set.
*/
#define BLUE_IN_SET 0

/**
@def OPACITY_IN_SET
@brief The default value for the opacity of a point in the Julia set.
*/
#define OPACITY_IN_SET 255


/**
@def RED_OUTOF_SET
@brief The default value for the red of a point outside of the Julia set.
*/
#define RED_OUTOF_SET 10

/**
@def GREEN_OUTOF_SET
@brief The default value for the green of a point outside of the Julia set.
*/
#define GREEN_OUTOF_SET 10

/**
@def BLUE_OUTOF_SET
@brief The default value for the blue of a point outside of the Julia set.
*/
#define BLUE_OUTOF_SET 30

/**
@def OPACITY_OUTOF_SET
@brief The default value for the opacity of a point outside of the Julia set.
*/
#define OPACITY_OUTOF_SET 255


/**
@def RED_DELTA
@brief The default amount to increment the red value of a point outside of the
Julia set by for each iteration it passed.
*/
#define RED_DELTA 1.6

/**
@def GREEN_DELTA
@brief The default amount to increment the green value of a point outside of the
Julia set by for each iteration it passed.
*/
#define GREEN_DELTA 0.8

/**
@def RED_DELTA
@brief The default amount to increment the blue value of a point outside of the
Julia set by for each iteration it passed.
*/
#define BLUE_DELTA 1.4

/**
@def OPACITY_DELTA
@brief The default amount to increment the opacity of a point outside of the
Julia set by for each iteration it passed.
*/
#define OPACITY_DELTA 0


/**
@fn drawJuliaSet
@brief Draws the Julia set described by colorMap.
@param colorMap The 2D array of colors describing each pixel in the window.
@param window Pointer to the renderer to draw with.
@param windowWidth The width of the window (in pixels).
@param windowHeight The height of the window (in pixels).
*/
void drawJuliaSet (SDL_Color **colorMap, SDL_Renderer *renderer, long windowWidth,
				   long windowHeight);

/**
@fn initializeSDL
@brief Initializes SDL to allow the use of its functions and features.
@param title The string title to name the window.
@param window Pointer to an uninitialized SDL_Window struct.
@param renderer Pointer to an uninitialized SDL_Renderer struct.
@return true if SDL is successfully initialized, false otherwise.
*/
bool initializeSDL (char *title, SDL_Window **window, SDL_Renderer **renderer,
					long windowWidth, long windowHeight);

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
				  int errorCode);

/**
@fn freeColorMap
@brief Frees a dynamically allocated color map.
@param colorMap The color map to be freed.
@param windowWidth The width of the color map.
@param windowHeight The height of the color map.
*/
void freeColorMap (SDL_Color ** colorMap, long windowWidth, long windowHeight);

/**
@fn newColorMap
@brief Dynamically allocates a windowWidth x windowHeight 2D array of colors.
@param windowWidth The width (in pixels) of the window.
@param windowHeight The height (in pixels) of the window.
@return A dynamically allocated 2D array of SDL_Color structs.
*/
SDL_Color ** newColorMap (long windowWidth, long windowHeight);



/**
@fn colorInSet
@brief Gives the color of points that are in the Julia set.
@return An SDL_Color struct representing the color of points in the Julia set.
*/
SDL_Color colorInSet ();

/**
@fn colorOutOfSet
@brief Gives the color of points that are outside of the Julia set.
@param stageEliminated The number of iterations completed before the point was
eliminated from the Julia set.
@return An SDL_Color struct representing the color of points outside of the 
Julia set.
*/
SDL_Color colorOutOfSet (int stageEliminated);


#endif /* DRAWING_H */