# UNDER CONSTRUCTION / NOT YET FUnCTIONAL
This is for `c++`, alternatively see [template for `c`](https://github.com/schmarx/sdl-template).
## SDL template for c++

A template that contains the initial boilerplate needed for applications reliant on simple 2-dimensional graphics.

This is particularly for computational applications that require visualisation or user input, such as simulations.

This comes with initialisation of the graphics window, and loads a font in as a texture for fast text rendering (the font used is loaded in the render.c initialisation function, and can easily be changed here).

It also includes basic square and circle textures, but custom textures can be included through image files with SDL.

## Makefile and installation
The following is needed:
- Have a `c++` installation.
- Have the SDL libraries installed (including SDL_image and SDL_ttf)

There is a provided Makefile, which assumes `g++` is used for compilation. If there are any issues with running this, the contents can always be used for inspiration to make a manual compilation command.

### Input arguments
The application currently takes arguments for entity count and window size, used as follows:

`make N=100 w=1000 h=800`

This runs the program with 100 particles, in a window with dimensions $1000 \times 800$ pixels.

The width and height can be omitted to run the application in full-screen.

## Extending the code
For simple applications it should be sufficient to mainly just make changes to:
 - the `update(...)` function in `update.cpp`, to make changes to the logic of the program
 - the `render(...)` function in `render.cpp`, for making changes to the visual output
 - the `obj` struct in `main.h`, and the intialisation thereof in `main.cpp`

When adding new files, remember to link them in the `Makefile` / compilation command.

## Timing
Timing macros are available, which simplifies the process of performance logging.

## Random number generator
The basic rand() function was used for the RNG. Three macros are made available:
- `rng(max)` gives a random integer between `0` and `max` (inclusive)
- `rngr(min, max)` gives a random integer between `min` and `max` (inclusive)
- `rngf()` gives a random float between `0` and `1`

This RNG should definitely be replaced if random numbers are very important to the application.

## User inputs
Basic user inputs are handled, including:
- `q` for quitting
- `ESC` for pausing
- `r` for re-initialising entities
- `g` for toggling debug mode
- `RIGHT` to time step forward (use when paused)
- `LEFT` to time step backward (use when paused)

It is very easy to add handlers for additional inputs in `main.cpp`.

## Vectors
The file `vec.h` exposes a class for working with vectors. This includes both 2D and 3D vectors (`vec2` and `vec3` respectively).

## Multithreading (not yet implemented)
The plan is to add multithreading so that the whole process doesn't have to be gone through every time a new project is set up.