#pragma once
#include "../../aquaUtils/pch.h"
#include "../../aquaUtils/utils/FileUtils.h";

#include <string.h>
#include <stdint.h>

#define CI_W 128
#define CI_H 96

#pragma pack(push, 1)
struct my_BITMAPFILEHEADER {
    uint16_t bfType;
    uint32_t bfSize;
    uint16_t bfReserved1;
    uint16_t bfReserved2;
    uint32_t bfOffBits;
};

struct my_BITMAPINFOHEADER {
    uint32_t biSize;
    int32_t  biWidth;
    int32_t  biHeight;
    uint16_t biPlanes;
    uint16_t biBitCount;
    uint32_t biCompression;
    uint32_t biSizeImage;
    int32_t  biXPelsPerMeter;
    int32_t  biYPelsPerMeter;
    uint32_t biClrUsed;
    uint32_t biClrImportant;
};
#pragma pack(pop)

typedef struct tagColor {
    unsigned char r;
    unsigned char g;
    unsigned char b;
} wColor;

#define Byte unsigned char

typedef struct tag_cImage {
	unsigned char color[CI_H][CI_W];
	int __notnull__;
} ChunkedImage;

typedef struct tagImageCache {
    wColor** raw;
    const char* name;
    int __notnull__;
} ImageCache;

ChunkedImage* loadChunkedImage(const char* filename, int filter, ImageCache* cache);
unsigned char* LoadBitmapFile(char* filename, BITMAPINFOHEADER* bitmapInfoHeader);