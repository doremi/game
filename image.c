#include <stdio.h>
#include <string.h>
#include "image.h"

#define OFF_X 12
#define OFF_Y 158

#define SKIP_X 9
#define SKIP_Y 15

void image_load(struct image *img, const char *file)
{
	FILE *fp = fopen(file, "rb");
	if (!fp)
		perror(file);
	fread(img->buf, img->width*img->height*PIXEL, 1, fp);
	fclose(fp);
}

void image_save(struct image *img, const char *file)
{
	FILE *fp = fopen(file, "wb");
	if (!fp)
		perror(file);
	fwrite(img->buf, img->width*img->height*PIXEL, 1, fp);
	fclose(fp);
}

void image_getblock(const struct image * const img, struct image *block, int ix, int iy)
{
	int i, j;
	unsigned char r, g, b;
	for (j = 0; j < BLOCK_Y; ++j) {
		for (i = 0; i < BLOCK_X; ++i) {
			getpixel(img, ix*(BLOCK_X+SKIP_X)+i+OFF_X, iy*(BLOCK_Y+SKIP_Y)+j+OFF_Y, &r, &g, &b);
//			if (r != 0)
//				printf("(%d, %d) = (%d, %d, %d)\n", i, j, r, g, b);
			setpixel(block, i, j, r, g, b);
		}
	}
}

void image_append(struct image *dst, const struct image * const src, int ix)
{
	memcpy(dst->buf + (src->width*src->height*PIXEL*ix), src->buf, (src->width*src->height*PIXEL));
}

unsigned int image_weight(struct image *src, struct image *dst)
{
	int i, j;
	unsigned int weight = 0;
	unsigned char r, g, b, r1, g1, b1;

	for (j = 0; j < src->height; j += 2) {
		for (i = 0; i < src->width; i += 2) {
			getpixel(src, i, j, &r, &g, &b);
			getpixel(dst, i, j, &r1, &g1, &b1);
			weight += (r1 - r) * (r1 - r);
		}
	}
	return weight;
}
