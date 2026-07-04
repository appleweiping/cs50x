// CS50x Problem Set 4 — Recover
//
// Recovers JPEGs from a raw "memory card" image. The card is scanned in 512-byte blocks;
// a new JPEG begins at any block whose first four bytes are ff d8 ff ex. Each recovered
// image is written to a sequentially numbered file 000.jpg, 001.jpg, ...
// Usage: ./recover card.raw

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// FAT filesystem block size.
const int BLOCK_SIZE = 512;

int main(int argc, char *argv[])
{
    // Accept a single command-line argument.
    if (argc != 2)
    {
        printf("Usage: ./recover IMAGE\n");
        return 1;
    }

    // Open the memory card file.
    FILE *card = fopen(argv[1], "rb");
    if (card == NULL)
    {
        printf("Could not open %s.\n", argv[1]);
        return 1;
    }

    uint8_t buffer[BLOCK_SIZE];
    FILE *img = NULL;
    int file_count = 0;
    char filename[8]; // "000.jpg" + '\0'

    // Read the card one block at a time.
    while (fread(buffer, sizeof(uint8_t), BLOCK_SIZE, card) == (size_t) BLOCK_SIZE)
    {
        // Detect the start of a new JPEG.
        bool is_jpeg = buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff &&
                       (buffer[3] & 0xf0) == 0xe0;

        if (is_jpeg)
        {
            // Close the previous JPEG, if any.
            if (img != NULL)
            {
                fclose(img);
            }

            // Open a new sequentially numbered JPEG.
            sprintf(filename, "%03i.jpg", file_count);
            img = fopen(filename, "wb");
            if (img == NULL)
            {
                fclose(card);
                printf("Could not create %s.\n", filename);
                return 1;
            }
            file_count++;
        }

        // While inside a JPEG, write the block out.
        if (img != NULL)
        {
            fwrite(buffer, sizeof(uint8_t), BLOCK_SIZE, img);
        }
    }

    // Close any open files.
    if (img != NULL)
    {
        fclose(img);
    }
    fclose(card);
    return 0;
}
