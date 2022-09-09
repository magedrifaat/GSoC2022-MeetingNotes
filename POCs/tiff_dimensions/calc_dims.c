#include <stdio.h>
#include <stdint.h>
#include <tiffio.h>

int main(int argc, char **argv)
{
    char *image_name;
    if (argc == 2)
    {
        image_name = argv[1];
        printf("Checking dimensions of %s\n", image_name);
    }
    else
    {
        printf("Usage: ./calc_dims IMAGE_NAME\n");
        return 1;
    }

    TIFF* tif = TIFFOpen(image_name, "r");
    if (tif)
    {
        int page_count = 0;
        do {
            page_count++;
        } while (TIFFReadDirectory(tif));

        printf("No of Pages = %d\n", page_count);

        for (int i = 0; i < page_count; i++)
        {
            TIFFSetDirectory(tif, i);
            uint32_t width, height;
            uint16_t compression, photometric, planar;
            uint32_t rows_per_strip;
            TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width);
            TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &height);
            TIFFGetField(tif, TIFFTAG_COMPRESSION, &compression);
            TIFFGetField(tif, TIFFTAG_PHOTOMETRIC, &photometric);
            TIFFGetField(tif, TIFFTAG_ROWSPERSTRIP, &rows_per_strip);
            if (!TIFFGetField(tif, TIFFTAG_PLANARCONFIG, &planar))
                printf("no planar\n");
            
            printf("Dimensions of Page %d: Width = %d, Height = %d\n",
                     i, width, height);
            printf("Compression = %d\n", compression);
            printf("Photometric = %d\n", photometric);
            printf("Planar = %d\n", planar);
            printf("RPS = %d\n", rows_per_strip);
            
            char *make;
            if (TIFFGetField(tif, TIFFTAG_MAKE, &make))
                printf("Make = %s\n", make);
            else
                printf("No make\n");

            // uint32_t strip_count = TIFFNumberOfStrips(tif);
            // printf("Number Of Strips = %d\n", strip_count);
            // uint64_t *stripByteCounts;
            // TIFFGetField(tif, TIFFTAG_STRIPBYTECOUNTS, &stripByteCounts);
            // for (uint32_t i = 0; i < strip_count; i++)
            // {
            //     printf("Strip %d count = %ld\n", i, stripByteCounts[i]);
            // }
        }

        TIFFClose(tif);
    }

    return 0;
}