# Bonus Project 3D Ray Tracer

## Sprint 1

In this sprint, an application will be built that renders a field of view in a 2D plane. Geometric objects can be placed by the programmer. The static field of view is visible to the user as console output. Multiple frames are generated as ASCII text files.

### Definition of Done

- In the language of linear algebra the following concepts are implemented:
    - A Camera
    - Rays Originating from Camera
    - One or more Obstacles
- A one-dimensional container representing the screen is implemented (choose a resolution)
- The distance from camera to obstacle is determined for each pixel in the screen (use linear algebra)
- The screen is shown to the user as console output (maybe using ASCII art?)

### How to build and run the code?
- Clone the repository
- Run 2DRT.cpp

## Sprint 2

In this sprint, user interaction will be implemented. For this the library SDL will be installed and included. The user can move his position on a 2D plane and adjust his field of view.
User can view the 3D environment and can move around the domain (similar to a first person game)

### Definition of Done

- The library SDL is installed
- Instead of showing the screen as console output, the screen is displayed in an SDL window
- The screen is updated at a fixed frequency (frame rate)
- Event handlers are implemented:
    - The X-Y position of the camera can be adjusted using the WASD keys
    - The field of view can be rotated left-right using the mouse

### How to build and run the code?
- Make sure you have SDL library installed. If not, install it by running the command given below, else proceed to the next steps
    - sudo apt-get install libsdl2-2.0
- Clone the repository
- Follow the below Command Line instructions (inside the directory where the repo has been cloned):
    - cd 2d-ray-tracer
    - mkdir build
    - cd build
    - cmake ..
    - make
    - ./2D_RAY_TRACER
![Screenshot_from_2022-01-13_09-50-39](/uploads/bb2e8ae2c8b891c187c7b15883de4b75/Screenshot_from_2022-01-13_09-50-39.png)

## Sprint 3
In this sprint the application will be profiled and its performance will be improved. Techniques for improving performance as covered by the lecture will be applied.

### Definion of "done"

- The average calculation time per frame is saved as a .txt file after the program is closed
- From this the average framerate is computed and saved in the .txt file
- Rate limiting steps of the program are identified
- The efficiency of the program is improved by enhancing these steps
