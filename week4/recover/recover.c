#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef uint8_t BYTE;

// Number of bytes in .jpeg blocks (FAT)
const int BLOCK_SIZE = 512;

// Prototypes
bool sig_pattern(BYTE *x);

int main(int argc, char *argv[])
{
    // single command-line argument
    if (argc != 2)
    {
        printf("Usage: ./recover card.raw\n");
        return 1;
    }

    // Open files
    FILE *input = fopen(argv[1], "r");
    if (input == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    BYTE block[BLOCK_SIZE];

    // to update file names
    int filenumber = 0;

    FILE *output = NULL;

    while (fread(&block, sizeof(BYTE), BLOCK_SIZE, input) != 0)
    {

        if (sig_pattern(block))
        {

            char filename[8];
            sprintf(filename, "%'.03d.jpg", filenumber);

            if (output != NULL)
            {
                fclose(output);
            }

            output = fopen(filename, "w");

            if (output == NULL)
            {
                printf("Could not open file.\n");
                return 1;
            }

            filenumber += 1;
        }

        if (output != NULL)
        {
            fwrite(&block, sizeof(BYTE), BLOCK_SIZE, output);
        }
    }

    fclose(input);
    fclose(output);
}

bool sig_pattern(BYTE *x)
{
    // function to check if the block has the jpeg start signature
    if ((x[0] == 255) && (x[1] == 216) && (x[2] == 255) && ((x[3] >= 224) && (x[3] <= 239)))
    {
        return true;
    }
    else
    {
        return false;
    }
}
