// Declarations of the four image filters for CS50x pset4/filter (more comfortable).

#include "bmp.h"

// Convert image to grayscale.
void grayscale(int height, int width, RGBTRIPLE image[height][width]);

// Reflect image horizontally.
void reflect(int height, int width, RGBTRIPLE image[height][width]);

// Blur image (3x3 box blur).
void blur(int height, int width, RGBTRIPLE image[height][width]);

// Detect edges (Sobel operator).
void edges(int height, int width, RGBTRIPLE image[height][width]);
