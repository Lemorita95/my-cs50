// Modifies the volume of an audio file

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// Number of bytes in .wav header
const int HEADER_SIZE = 44;

int main(int argc, char *argv[])
{
    // Check command-line arguments
    if (argc != 4)
    {
        printf("Usage: ./volume input.wav output.wav factor\n");
        return 1;
    }

    // Open files and determine scaling factor
    FILE *input = fopen(argv[1], "r");
    if (input == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    FILE *output = fopen(argv[2], "w");
    if (output == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    float factor = atof(argv[3]);

    // TODO: Copy header from input file to output file
    uint8_t header[HEADER_SIZE]; // region of memory which data read will be stored

    // store in 'header' memory region, HEADER_SIZE bytes, 1 unity, read from 'input'
    fread(&header, HEADER_SIZE, 1, input);
    // read from 'header' memory region, HEADER_SIZE bytes, 1 unity, write in 'output'
    fwrite(&header, HEADER_SIZE, 1, output);

    // TODO: Read samples from input file and write updated data to output file
    int16_t sample;

    // loop throught each sample until there is no samples left to read
    while (fread(&sample, sizeof(int16_t), 1, input) != 0)
    {
        // scaling sample value
        sample *= factor;
        fwrite(&sample, sizeof(int16_t), 1, output);
    }

    // Close files
    fclose(input);
    fclose(output);
}
