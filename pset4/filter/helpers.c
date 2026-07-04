// CS50x Problem Set 4 — filter helpers (grayscale, reflect, blur, edges).

#include "helpers.h"

#include <math.h>
#include <string.h>

// Clamp a channel value to the valid [0, 255] range.
static BYTE clamp(long value)
{
    if (value < 0)
    {
        return 0;
    }
    if (value > 255)
    {
        return 255;
    }
    return (BYTE) value;
}

// Convert image to grayscale by setting each pixel's R, G, B to the rounded average.
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            RGBTRIPLE p = image[i][j];
            int avg = (int) round((p.rgbtRed + p.rgbtGreen + p.rgbtBlue) / 3.0);
            image[i][j].rgbtRed = image[i][j].rgbtGreen = image[i][j].rgbtBlue = (BYTE) avg;
        }
    }
}

// Reflect image horizontally by swapping pixels across the vertical midline.
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width / 2; j++)
        {
            RGBTRIPLE tmp = image[i][j];
            image[i][j] = image[i][width - 1 - j];
            image[i][width - 1 - j] = tmp;
        }
    }
}

// Blur image with a 3x3 box blur: each output pixel is the average of itself and its
// (up to eight) neighbours, computed from a copy of the original image.
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE copy[height][width];
    memcpy(copy, image, sizeof(RGBTRIPLE) * height * width);

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            long sumR = 0, sumG = 0, sumB = 0;
            int count = 0;
            for (int di = -1; di <= 1; di++)
            {
                for (int dj = -1; dj <= 1; dj++)
                {
                    int ni = i + di;
                    int nj = j + dj;
                    if (ni >= 0 && ni < height && nj >= 0 && nj < width)
                    {
                        sumR += copy[ni][nj].rgbtRed;
                        sumG += copy[ni][nj].rgbtGreen;
                        sumB += copy[ni][nj].rgbtBlue;
                        count++;
                    }
                }
            }
            image[i][j].rgbtRed = (BYTE) round((double) sumR / count);
            image[i][j].rgbtGreen = (BYTE) round((double) sumG / count);
            image[i][j].rgbtBlue = (BYTE) round((double) sumB / count);
        }
    }
}

// Detect edges using the Sobel operator. For each pixel, Gx and Gy are computed per
// channel over the 3x3 neighbourhood (treating out-of-bounds pixels as black), and the
// output channel is sqrt(Gx^2 + Gy^2) clamped to [0, 255].
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE copy[height][width];
    memcpy(copy, image, sizeof(RGBTRIPLE) * height * width);

    // Sobel kernels.
    int Gx[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    int Gy[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            long gxR = 0, gxG = 0, gxB = 0;
            long gyR = 0, gyG = 0, gyB = 0;

            for (int di = -1; di <= 1; di++)
            {
                for (int dj = -1; dj <= 1; dj++)
                {
                    int ni = i + di;
                    int nj = j + dj;
                    if (ni >= 0 && ni < height && nj >= 0 && nj < width)
                    {
                        RGBTRIPLE p = copy[ni][nj];
                        int kx = Gx[di + 1][dj + 1];
                        int ky = Gy[di + 1][dj + 1];
                        gxR += p.rgbtRed * kx;
                        gxG += p.rgbtGreen * kx;
                        gxB += p.rgbtBlue * kx;
                        gyR += p.rgbtRed * ky;
                        gyG += p.rgbtGreen * ky;
                        gyB += p.rgbtBlue * ky;
                    }
                }
            }

            image[i][j].rgbtRed = clamp(lround(sqrt((double) gxR * gxR + (double) gyR * gyR)));
            image[i][j].rgbtGreen = clamp(lround(sqrt((double) gxG * gxG + (double) gyG * gyG)));
            image[i][j].rgbtBlue = clamp(lround(sqrt((double) gxB * gxB + (double) gyB * gyB)));
        }
    }
}
