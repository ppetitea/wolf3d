#include <stdio.h>
#include <stdlib.h>
#define BMP_HEADER_SIZE 54
#define DIB_HEADER_SIZE 40
#pragma pack(push)  // save the original data alignment
#pragma pack(1)
typedef int int32;
typedef short int16;
typedef int int32_t;
typedef unsigned int uint32_t;
typedef unsigned short uint16_t;

typedef struct 		s_bitmap_header
{
    char			type[2];           	// Magic identifier: 0x4d42
    uint32_t		size;              // File size in bytes
    uint16_t		reserved1;         // Not used
    uint16_t		reserved2;         // Not used
    uint32_t 		offset;            // Offset to image data in bytes from beginning of file
    uint32_t		header_size;   	// Header size in bytes
    int32_t			width;          // Width of the image
    int32_t			height;         // Height of image
    uint16_t		num_planes;        // Number of color planes
    uint16_t		bits_per_pixel;    // Bits per pixel
    uint32_t		compression;       // Compression type
    uint32_t		image_size_bytes;  // Image size in bytes
    int32_t			x_resolution_ppm;  // Pixels per meter
    int32_t			y_resolution_ppm;  // Pixels per meter
    uint32_t		num_colors;        // Number of colors
    uint32_t		important_colors;  // Important colors
} 					t_bitmap_header;

#pragma pack(pop)  // restore the previous pack setting

typedef struct		s_bitmap_texture
{
    t_bitmap_header head;
    uint32_t		*pixels;
}					t_bitmap_texture;

u_int32_t	*fill_bitmap_texture(t_bitmap_texture *bmp, FILE *file)
{
	int		i;
	int		j;
	int		w;
	int		h;
	unsigned char	bgr_pixel[4];

	w = bmp->head.width;
	h = bmp->head.height;
	if (!(bmp->pixels = malloc(sizeof(w * h))))
		return (-1);
	j = -1;
	while (++j < h)
	{
		i = -1;
		while (++i < w)
		{
			fread(&bgr_pixel, 1, 3, file);
			bmp->pixels[i + (h - j - 1) * w] = bgr_to_rgb_pixel(&bgr_pixel);
		}
	}
}

int		main(int ac, char **av)
{
	FILE				*file;
	t_bitmap_texture	bmp;

	file = NULL;
	if (ac != 2 || !(file = fopen(av[1], "rb")))
		printf("error !\n");
	fread(&bmp.head, sizeof(t_bitmap_header), 1, file);
	bmp.pixels = fill_bitmap_texture(&bmp, file);
	

	return (0);
}

// TODO :
// --> learn pragma pack(truc); enleve le padding pour que les variables soient alignees en memoire dans la struct
// --> how to store image
// --> 


	// printf("type %c%c\n", bmp.head.type[0], bmp.head.type[1]);
	// printf("size %d\n", bmp.head.size);
	// printf("offset %d\n", bmp.head.offset);
	// printf("header size %d\n", bmp.head.header_size);
	// printf("width %d\n", bmp.head.width);
	// printf("height %d\n", bmp.head.height);
	// printf("bpp %hd\n", bmp.head.bits_per_pixel);