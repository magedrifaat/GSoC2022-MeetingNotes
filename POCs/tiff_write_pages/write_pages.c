#include <stdio.h>
#include <stdint.h>

#include <tiffio.h>

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        printf("Usage: ./write_pages page_count\n");
        return 1;
    }

    int page_count;
    sscanf(argv[1], "%d", &page_count);

    TIFF *tif = TIFFOpen("test_bilevel.tif", "w");
    if (tif)
    {
        for (int i = 0; i < page_count; i++)
        {
            TIFFCreateDirectory(tif);
            TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, 10);
            TIFFSetField(tif, TIFFTAG_IMAGELENGTH, 10);
            TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_MINISWHITE);
            TIFFSetField(tif, TIFFTAG_COMPRESSION, COMPRESSION_NONE);
            TIFFSetField(tif, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
            TIFFSetField(tif, TIFFTAG_MAKE, "MakeTest");

            TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, 1);
            TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, 1);

            TIFFSetField(tif, TIFFTAG_PAGENUMBER, 1, 1);

            // uint32_t pixel = (128 << 16) | (120 << 8) | 70;
            // uint64_t two_pixels = (((uint64_t)pixel)<<24) + pixel;
            uint16_t rows[10] = {0b1100000011100001, 0b1100000011100001,
                                 0b1100000011100001, 0b1100000011100001,
                                 0b1100000011100001, 0b1100000011100001,
                                 0b1100000011100001, 0b1100000011100001,
                                 0b1100000011000001, 0b1100000011000001};
            TIFFWriteRawStrip(tif, 0, rows, 20);
            TIFFFlush(tif);
        }
        TIFFClose(tif);
    }

    return 0;
}