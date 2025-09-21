# UNDER CONSTRUCTION / NOT YET FUNCTIONAL
This is for `c++`, alternatively see [template for `c`](https://github.com/schmarx/sdl-template).
## SDL template for c++

A template that contains the initial boilerplate needed for applications reliant on simple 2-dimensional graphics.

This is particularly for computational applications that require visualisation or user input, such as simulations.

This comes with initialisation of the graphics window, and loads a font in as a texture for fast text rendering (the font used is loaded in the `src/lib/render.c` initialisation function, and can easily be changed here).

It also includes basic square and circle textures, but custom textures can be included through image files with SDL.

## Makefile and installation
The following is needed:
- Have a `c++` installation.
- Have the SDL libraries installed (including SDL_ttf for text rendering)

There is a provided Makefile, which assumes `g++` is used for compilation. If there are any issues with running this, the contents can always be used for inspiration to make a manual compilation command.

### Input arguments
The application uses a configuration file `config.txt` in the root directory. Any parameter can be ignored by setting its value to `-1`. The following parameters can be assigned:
- `fps` to cap the frame rate of the program (defaults to uncapped)
- `N` to set the object count (this will be overridden if `N` is passed as a command-line argument)
- `seed` to set the RNG seed to a specific value (eg. when debugging specific conditions)
- `sim_scale` multiplies the display size by this value to determine simulation size (see section below on simulation scaling)
- `margin_left`, `margin_right`, `margin_top`, `margin_bottom` sets the corresponding margins in the display window, so that the simulation only runs in part of the window
- `window_x`, `window_y` sets the window size in pixels (defaults to full screen)

The command-line arguments for entity count and window size will override the ones provided in `config.txt`, and is, used as follows:

`make N=100 w=1000 h=800`

This runs the program with 100 particles, in a window with dimensions $1000 \times 800$ pixels.

The parameters can be omitted to run the application as per the `config.txt` file.

### Simulation scaling
By default the simulation runs in full screen, with each pixel corresponding to one unit in the simulation.

Sometimes you may want to use portions of the window for UI for example, and thus you may want to localise the simulation to a specific portion of the window.

For this the four `margin` properties can be set in `config.txt`, corresponding to the pixel offset from the respesctive sides of the window.

To scale the simulation so that a pixel corresponds to an arbitrary amount of units in the simulation, the `sim_scale` parameter can be set.

The simulation dimensions are calculated as follows:
$\begin{align}
    \text{sim}_x = (\text{win}_x - \text{margin}_x) \times \text{sim\_scale} \\
    \text{sim}_y = (\text{win}_y - \text{margin}_y) \times \text{sim\_scale}
\end{align}$
Where $\text{margin}_x$ and $\text{margin}_y$ are the sums of the x-margins, and y-margins respectively.

## Extending the code
For simple applications it should be sufficient to mainly just make changes to:
 - the `update(...)` function in `update.cpp`, to make changes to the logic of the program
 - the `render(...)` function in `render.cpp`, for making changes to the visual output
 - the `obj` class in `main.h`, and the intialisation thereof in `main.cpp`

There will rarely be any need to change anything in the files contained in `src/lib`, but the most likely things to change will be the `app_init(...)` and `app_run()` functions in `src/lib/app.cpp`.

When adding new files, remember to link them in the `Makefile` / compilation command.

## Timing
Timing macros are available, which simplifies the process of performance logging.

## Random number generator
The basic rand() function was used for the RNG. Three macros are made available:
- `rng(max)` gives a random integer between `0` and `max` (inclusive)
- `rngr(min, max)` gives a random integer between `min` and `max` (inclusive)
- `rngf()` gives a random float between `0` and `1`
- `rngfr(min, max)` gives a random float between `min` and `max`

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