#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include <tiffio.h>

void valid(int res)
{
    if (!res)
    {
        printf("valid failed\n");
        *((int *) 0) = 1;
    }

}

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        printf("Usage: ./convert image_to_convert.tif\n");
        return 1;
    }

    char *name_in = argv[1];
    char name_out[100];
    int start_index;
    for (start_index = strlen(name_in) - 1;
         start_index >= 0 && name_in[start_index] != '/';
         start_index--);
    start_index += 1;

    for (int i = start_index; i < strlen(name_in) - 4; i++)
        name_out[i - start_index] = name_in[i];
    
    char *suffix = "_tiled.tif";
    for (int i = 0; i <= strlen(suffix); i++)
        name_out[strlen(name_in) - start_index - 4 + i] = suffix[i];

    TIFF *image_in = TIFFOpen(name_in, "r");
    TIFF *image_out = TIFFOpen(name_out, "w");

    if (!image_in || !image_out)
    {
        printf("Failed to open a file\n");
        return 1;
    }

    uint32_t width, height;
    uint16_t planar, compression, bitspersample, samplesperpixel, photometric;
    valid(TIFFGetField(image_in, TIFFTAG_IMAGEWIDTH, &width));
    valid(TIFFGetField(image_in, TIFFTAG_IMAGELENGTH, &height));
    valid(TIFFGetField(image_in, TIFFTAG_BITSPERSAMPLE, &bitspersample));
    valid(TIFFGetField(image_in, TIFFTAG_SAMPLESPERPIXEL, &samplesperpixel));
    valid(TIFFGetField(image_in, TIFFTAG_COMPRESSION, &compression));
    valid(TIFFGetField(image_in, TIFFTAG_PHOTOMETRIC, &photometric));
    valid(TIFFGetField(image_in, TIFFTAG_PLANARCONFIG, &planar));
    
    valid(TIFFSetField(image_out, TIFFTAG_IMAGEWIDTH, width));
    valid(TIFFSetField(image_out, TIFFTAG_IMAGELENGTH, height));
    valid(TIFFSetField(image_out, TIFFTAG_BITSPERSAMPLE, bitspersample));
    valid(TIFFSetField(image_out, TIFFTAG_SAMPLESPERPIXEL, samplesperpixel));
    valid(TIFFSetField(image_out, TIFFTAG_COMPRESSION, compression));
    valid(TIFFSetField(image_out, TIFFTAG_PHOTOMETRIC, photometric));
    valid(TIFFSetField(image_out, TIFFTAG_PLANARCONFIG, planar));
    
    uint32_t rows_per_strip;
    valid(TIFFGetField(image_in, TIFFTAG_ROWSPERSTRIP, &rows_per_strip));
    
    uint32_t tile_width = 16;
    uint32_t tile_height = 32;
    valid(TIFFSetField(image_out, TIFFTAG_TILEWIDTH, tile_width));
    valid(TIFFSetField(image_out, TIFFTAG_TILELENGTH, tile_height));

    tdata_t buf_in = _TIFFmalloc(TIFFStripSize(image_in));
    tdata_t buf_out = _TIFFmalloc(TIFFTileSize(image_out));
    _TIFFmemset(buf_out, 0, TIFFTileSize(image_out));

    uint32_t tile_count = TIFFNumberOfTiles(image_out);

    void *tile_mem[tile_count];
    for (uint32_t i = 0; i < tile_count; i++)
    {
        tile_mem[i] = calloc(1, TIFFTileSize(image_out));
    }

    uint32_t base_row = 0;
    for (uint32_t strip = 0; strip < TIFFNumberOfStrips(image_in); strip++)
    {
        int64_t strip_bytes = TIFFReadEncodedStrip(image_in, strip, buf_in, -1);
        if (strip_bytes == -1)
        {
            printf("failed to read strip data\n");
            return 1;
        }

        uint32_t rows_in_strip = strip_bytes * 8 / (width * bitspersample);
        if (planar == PLANARCONFIG_CONTIG)
            rows_in_strip /= samplesperpixel;
        for (uint32_t row = 0; row < rows_in_strip; row++)
        {
            for (uint32_t col = 0; col < width; col++)
            {
                if (planar == PLANARCONFIG_CONTIG)
                {
                    uint32_t pixel_size = samplesperpixel * bitspersample / 8;
                    uint32_t tile = TIFFComputeTile(image_out, col, base_row + row, 0, 0);
                    memcpy(tile_mem[tile] + tile_width * ((base_row + row) % tile_height) * pixel_size + (col % tile_width) * pixel_size,
                           buf_in + row * width * pixel_size + col * pixel_size, pixel_size);
                }
                else
                {
                    uint32_t pixel_size = bitspersample / 8;
                    uint16_t sample = base_row / height;
                    uint32_t tile = TIFFComputeTile(image_out, col, base_row % height + row, 0, sample);
                    memcpy(tile_mem[tile] + tile_width * ((base_row % height + row) % tile_height) * pixel_size + (col % tile_width) * pixel_size,
                           buf_in + row * width * pixel_size + col * pixel_size, pixel_size);
                }
            }
        }
        base_row += rows_in_strip;
    }
    for (uint32_t tile = 0; tile < tile_count; tile++)
    {
        TIFFWriteEncodedTile(image_out, tile, tile_mem[tile], -1);
    }

    TIFFFlush(image_out);

    TIFFClose(image_in);
    TIFFClose(image_out);

    _TIFFfree(buf_in);
    _TIFFfree(buf_out);
}