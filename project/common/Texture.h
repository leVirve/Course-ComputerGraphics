#pragma once

extern "C" {

#pragma pack(push, 2)
typedef struct _FileHeader
{
    unsigned char ID[2];
    unsigned long FileSizepr;
    unsigned long Reserved;
    unsigned long DataOffset;
} FileHeader;

typedef struct _InfoHeader
{
    unsigned long HeaderSize;
    unsigned long Width;
    unsigned long Height;
    unsigned short Planes;
    unsigned short BitsPerPixel;
    unsigned long Compression;
    unsigned long BmpDataSize;
    unsigned long HResolusion;
    unsigned long VResolusion;
    unsigned long UsedColor;
    unsigned long ImportantColors;
} InfoHeader;
#pragma pack(pop)

}
