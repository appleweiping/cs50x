// CS50x Problem Set 4 — Volume
//
// Scales the volume of a 16-bit mono WAV file by a given factor. The WAV header (first 44
// bytes) is copied verbatim; each subsequent 16-bit little-endian sample is multiplied by
// the factor and written out. Usage: ./volume input.wav output.wav factor

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// Number of bytes in .wav header.
const int HEADER_SIZE = 44;

int main(int argc, char *argv[])
{
    // Check command-line arguments.
    if (argc != 4)
    {
        printf("Usage: ./volume input.wav output.wav factor\n");
        return 1;
    }

    // Open files and determine scaling factor.
    FILE *input = fopen(argv[1], "rb");
    if (input == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    FILE *output = fopen(argv[2], "wb");
    if (output == NULL)
    {
        fclose(input);
        printf("Could not open file.\n");
        return 1;
    }

    float factor = atof(argv[3]);

    // Copy the 44-byte header verbatim.
    uint8_t header[HEADER_SIZE];
    fread(header, sizeof(uint8_t), HEADER_SIZE, input);
    fwrite(header, sizeof(uint8_t), HEADER_SIZE, output);

    // Read each 16-bit sample, scale it, and write it out.
    int16_t sample;
    while (fread(&sample, sizeof(int16_t), 1, input) == 1)
    {
        sample = (int16_t) (sample * factor);
        fwrite(&sample, sizeof(int16_t), 1, output);
    }

    fclose(input);
    fclose(output);
    return 0;
}
