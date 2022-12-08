#include "Image.h"

ChunkedImage* loadChunkedImage(const char* filename, int f, ImageCache* cache)
{
    ChunkedImage* img = (ChunkedImage*)malloc(sizeof(ChunkedImage));
    if (img == 0) return panic("Memory error!");
    if (cache == 0) return panic("panic!");
    if (cache->__notnull__ == 0 || (strcmp(cache->name, filename) != 0)) {
        BITMAPINFOHEADER bitmap;
        Byte* raw = LoadBitmapFile(filename, &bitmap);
        if (bitmap.biWidth != CI_W) panic("Bad resolution!");
        if (bitmap.biHeight != CI_H) panic("Bad resolution!");
        wColor** raw_i = (int*)malloc(sizeof(wColor*) * CI_H);
        if (raw_i == 0) return panic("Memory error!");
        for (int i = 0; i < CI_H; i++) raw_i[i] = (wColor*)malloc(sizeof(wColor) * CI_W);
        for (int i = 0; i < CI_H; i++) {
            for (int j = 0; j < CI_W; j++) {
                if (raw_i[i] == 0) return panic("Memory error!");
                raw_i[i][j].r = raw[i * CI_W * 3 + j * 3];
                raw_i[i][j].g = raw[i * CI_W * 3 + j * 3 + 1];
                raw_i[i][j].b = raw[i * CI_W * 3 + j * 3 + 2];
            }
        }
        if (raw == 0) return panic("Bad file!");
        cache->__notnull__ = 1;
        cache->raw = raw_i;
        cache->name = SUS_str_copy(filename);
    }
    //printf("\n %dx%d = %d", bitmap.biWidth, bitmap.biHeight, bitmap.biSize);
    for (int i = 0; i < CI_H; i++) {
        for (int j = 0; j < CI_W; j++) {
            //todo 
            img->color[i][j] = 0;
            if (cache->raw[i][j].r > f) img->color[i][j] += 4;
            if (cache->raw[i][j].g > f) img->color[i][j] += 2;
            if (cache->raw[i][j].b > f) img->color[i][j] += 1;
            if (cache->raw[i][j].r > 128 || cache->raw[i][j].g > 128 || cache->raw[i][j].b > 128) {
                img->color[i][j] += 8;
            }
        }
    }
    return img;
}

Byte* LoadBitmapFile(char* filename, BITMAPINFOHEADER* bitmapInfo)
{
    FILE* filePtr;
    BITMAPFILEHEADER bitmapFileHeader;  
    unsigned char* bitmapImage;  
    int imageIdx = 0;  
    unsigned char tempRGB;

    fopen_s(&filePtr, filename, "rb");
    if (filePtr == NULL)
        return 0;

    fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, filePtr);

    if (bitmapFileHeader.bfType != 0x4D42)
    {
        fclose(filePtr);
        return 0;
    }

    fread(bitmapInfo, sizeof(BITMAPINFOHEADER), 1, filePtr);
    fseek(filePtr, bitmapFileHeader.bfOffBits, SEEK_SET);
    bitmapImage = (unsigned char*)malloc(bitmapInfo->biSizeImage);

    if (!bitmapImage)
    {
        free(bitmapImage);
        fclose(filePtr);
        return 0;
    }

    fread(bitmapImage, bitmapInfo->biSizeImage, 1, filePtr);
    if (bitmapImage == 0)
    {
        fclose(filePtr);
        return 0;
    }
    //maybe this is ok
    for (imageIdx = 0; imageIdx < bitmapInfo->biSizeImage; imageIdx += 3)
    {
        tempRGB = bitmapImage[imageIdx];
        bitmapImage[imageIdx] = bitmapImage[imageIdx + 2];
        bitmapImage[imageIdx + 2] = tempRGB;
    }

    fclose(filePtr);
    return bitmapImage;
}
