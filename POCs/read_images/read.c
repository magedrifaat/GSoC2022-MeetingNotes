#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <tiffio.h>

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        printf("Usage: ./read filename\n");
        return 1;
    }

    char *filename = argv[1];
    TIFF *t = TIFFOpen(filename, "r");

    if (t)
    {
        uint32_t width, height;
        TIFFGetField(t, TIFFTAG_IMAGEWIDTH, &width);
        TIFFGetField(t, TIFFTAG_IMAGELENGTH, &height);
        uint16_t samples_per_pixel;
        TIFFGetField(t, TIFFTAG_SAMPLESPERPIXEL, &samples_per_pixel);

        uint16_t bits_per_sample;
        TIFFGetField(t, TIFFTAG_BITSPERSAMPLE, &bits_per_sample);

        tdata_t buf;
        tstrip_t strip;
        
        buf = _TIFFmalloc(TIFFStripSize(t));
        
        for (strip = 0; strip < TIFFNumberOfStrips(t); strip++)
        {
            uint32_t strip_bytes = TIFFReadEncodedStrip(t, strip, buf, (tsize_t) -1);
            uint32_t rows_in_strip = strip_bytes / (width * samples_per_pixel * bits_per_sample / 8);
            for (uint32_t i = 0; i < rows_in_strip; i++)
            {
                for (uint32_t j = 0; j < width * samples_per_pixel * bits_per_sample / 8; j++)
                    printf("%d ", ((uint8_t *)buf)[i * width + j]);
                printf("\n");
            }

        }
        // uint16_t *red, *green, *blue;
        // TIFFGetField (t, TIFFTAG_COLORMAP, &red, &green, &blue);
        // uint32_t count = 1 << bits_per_sample;

        // for (uint32_t i = 0; i < count; i++)
            // printf("(%d, %d, %d)\n", red[i], green[i], blue[i]);
        
        _TIFFfree(buf);
        TIFFClose(t);
    }
    else
    {

        printf("Couldn't open file\n");
        return 1;
    }
}