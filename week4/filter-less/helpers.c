#include <math.h>

#include "helpers.h"

int int_min(int a, int b);

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    // loop througt columns (height)
    for (int i = 0; i < height; i++)
    {
        // lopp throught rows (width)
        for (int j = 0; j < width; j++)
        {
            int c =
                round((image[i][j].rgbtBlue + image[i][j].rgbtGreen + image[i][j].rgbtRed) / 3.0);

            image[i][j].rgbtBlue = c;
            image[i][j].rgbtGreen = c;
            image[i][j].rgbtRed = c;
        }
    }
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    // loop througt columns (height)
    for (int i = 0; i < height; i++)
    {
        // lopp throught rows (width)
        for (int j = 0; j < width; j++)
        {
            int sr = round(0.393 * image[i][j].rgbtRed + 0.769 * image[i][j].rgbtGreen +
                           0.189 * image[i][j].rgbtBlue);
            int sg = round(0.349 * image[i][j].rgbtRed + 0.686 * image[i][j].rgbtGreen +
                           0.168 * image[i][j].rgbtBlue);
            int sb = round(0.272 * image[i][j].rgbtRed + 0.534 * image[i][j].rgbtGreen +
                           0.131 * image[i][j].rgbtBlue);

            image[i][j].rgbtRed = int_min(sr, 255);
            image[i][j].rgbtGreen = int_min(sg, 255);
            image[i][j].rgbtBlue = int_min(sb, 255);
        }
    }
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    // loop througt columns (height)
    for (int i = 0; i < height; i++)
    {
        // lopp throught rows (width)
        for (int j = 0; j < (width / 2); j++)
        {
            RGBTRIPLE tmp = image[i][j];
            image[i][j] = image[i][width - 1 - j];
            image[i][width - 1 - j] = tmp;
        }
    }
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // loop througt columns (height)
    RGBTRIPLE copy[height][width];
    for (int i = 0; i < height; i++)
    {
        // lopp throught rows (width)
        for (int j = 0; j < width; j++)
        {
            copy[i][j] = image[i][j];
        }
    }

    for (int i = 0; i < height; i++)
    {
        // lopp throught rows (width)
        for (int j = 0; j < width; j++)
        {
            int ar = 0;
            int ag = 0;
            int ab = 0;
            int px = 0;

            // adjacent rowise iteration
            for (int k = -1; k <= 1; k++)
            {
                // adjascent columnwise iteration
                for (int l = -1; l <= 1; l++)
                {
                    // to deal with edges
                    if ((((i + k) >= 0) && ((i + k) < height)) &&
                        (((j + l) >= 0) && ((j + l) < width)))
                    {
                        ar += copy[i + k][j + l].rgbtRed;
                        ag += copy[i + k][j + l].rgbtGreen;
                        ab += copy[i + k][j + l].rgbtBlue;
                        px += 1;
                    }
                }
            }

            image[i][j].rgbtBlue = round(ab / (float) px);
            image[i][j].rgbtGreen = round(ag / (float) px);
            image[i][j].rgbtRed = round(ar / (float) px);
        }
    }
}

int int_min(int a, int b)
{
    int m = a;

    if (b < a)
    {
        m = b;
    }

    return m;
}
