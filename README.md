# Animal-Skin-Pattern-Generator
C++ implementation of an Activator-Inhibitor Cellular Automata, used for generating images that reflect many natural forming patterns, such as zebra skin. 

This process starts with randomly setting a grid of "cells" to each have a value of either 0 or 1. Each cell is randomly picked and updated based on a rule that will either change or not change the value of the cell. When the grid converges to a stable state or the max number of steps has been reached, a PGM image of the grid is generated. 

## Examples of Generated Images:
![Crazy Pattern](/examples/crazy-pattern.jpg)
![Dalmatian](/examples/dalmatian.jpg)
![Dots](/examples/small-dots.jpg)
![Other](/examples/other.jpg)
  

Step 1)  Compile CA.cpp using g++ (no other libraries needed)

Step 2)  Execute script. Using no command line args, parameter values are pre-set to an interesting pattern. To input your own custom parameters, include them as command line arguments. 

    USAGE: double h, double J1, double J2, int R1, int R2, (optional int maxSteps) string outputFilename

## Example Parameter Settings:

  Dalmatian:        2 1 -1 4 9
  
  Crazy Pattern:    1.5 0.5 -0.5 10 14
  
  Blobs:            1.5 0.5 -0.5 10 15
