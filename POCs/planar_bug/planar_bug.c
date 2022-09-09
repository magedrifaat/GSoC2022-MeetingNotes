#include <stdio.h>

#include <tiffio.h>

int main(void)
{
    TIFF *tif = TIFFOpen("test.tif", "w");

    unsigned short planar;
    TIFFGetFieldDefaulted(tif, TIFFTAG_PLANARCONFIG, &planar);
    printf("Default Planar value = %d\n", planar);

    TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, 1);
    TIFFSetField(tif, TIFFTAG_IMAGELENGTH, 1);
    TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, 3);
    TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, 8);
    TIFFSetField(tif, TIFFTAG_PLANARCONFIG, 1);
    unsigned char pixel[3] = {255, 255, 255};
    TIFFWriteEncodedStrip(tif, 0, pixel, 3);
    TIFFClose(tif);
}