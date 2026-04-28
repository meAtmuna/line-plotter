# Line Plotter (C)
This is a terminal based graph plotting program written in C. it takes user input for either a linear function or a quadratic function and displays it using ASCII characters.

## Features:
* Supports line (y = mx + c)
* Supports Quadratic (y = ax² + bx + c)
* Option to plot a second function for comparison
* Automatic graph scaling sor graph fits the screen
* Simple zoom control
* Shows minimum and maximum values on the graph

## How to Run:
* Compile
  - gcc line_plotter.c -o line_plotter
* Run
  - ./line_plotter

## How It Works:
* You choose the type of function
* Enter values (slope, intercept, or a, b, c)
* Program calcultes points and scales them
* Graph is down in terminal using charcters:
    - o = first function
    - x = second function

## Notes
* Works best in a normal terminal (Linux/macOs or Windows CMD)
* Users ANSI escape codes to control screen