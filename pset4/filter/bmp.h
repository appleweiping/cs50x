// BMP-related data types based on Microsoft's own (from CS50's distribution).
//
// A BMP file consists of a BITMAPFILEHEADER, a BITMAPINFOHEADER, then pixel data of type
// RGBTRIPLE (blue, green, red). This header defines the fixed-width types and the two
// header structs for the 24-bit uncompressed BMP 4.0 format used by pset4/filter.

#include <stdint.h>

// These data types are essentially aliases for C/C++ primitive data types.
typedef uint8_t BYTE;
typedef uint32_t DWORD;
typedef int32_t LONG;
typedef uint16_t WORD;

// BITMAPFILEHEADER — see https://learn.microsoft.com/windows/win32/api/wingdi/ns-wingdi-bitmapfileheader
typedef struct
{
    WORD bfType;
    DWORD bfSize;
    WORD bfReserved1;
    WORD bfReserved2;
    DWORD bfOffBits;
} __attribute__((__packed__)) BITMAPFILEHEADER;

// BITMAPINFOHEADER — see https://learn.microsoft.com/windows/win32/api/wingdi/ns-wingdi-bitmapinfoheader
typedef struct
{
    DWORD biSize;
    LONG biWidth;
    LONG biHeight;
    WORD biPlanes;
    WORD biBitCount;
    DWORD biCompression;
    DWORD biSizeImage;
    LONG biXPelsPerMeter;
    LONG biYPelsPerMeter;
    DWORD biClrUsed;
    DWORD biClrImportant;
} __attribute__((__packed__)) BITMAPINFOHEADER;

// RGBTRIPLE — a single pixel, stored blue-green-red as in the BMP format.
typedef struct
{
    BYTE rgbtBlue;
    BYTE rgbtGreen;
    BYTE rgbtRed;
} __attribute__((__packed__)) RGBTRIPLE;
