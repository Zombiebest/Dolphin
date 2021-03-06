// Copyright (C) 2003 Dolphin Project.

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, version 2.0.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License 2.0 for more details.

// A copy of the GPL 2.0 should have been included with the program.
// If not, see http://www.gnu.org/licenses/

// Official SVN repository and contact information can be found at
// http://code.google.com/p/dolphin-emu/

#include <list>
#include <vector>

#include "ImageWrite.h"
#include "FileUtil.h"

#pragma pack(push, 1)

struct TGA_HEADER
{
    u8  identsize;          // size of ID field that follows 18 u8 header (0 usually)
    u8  colourmaptype;      // type of colour map 0=none, 1=has palette
    u8  imagetype;          // type of image 0=none,1=indexed,2=rgb,3=grey,+8=rle packed

    s16 colourmapstart;     // first colour map entry in palette
    s16 colourmaplength;    // number of colours in palette
    u8  colourmapbits;      // number of bits per palette entry 15,16,24,32

    s16 xstart;             // image x origin
    s16 ystart;             // image y origin
    s16 width;              // image width in pixels
    s16 height;             // image height in pixels
    u8  bits;               // image bits per pixel 8,16,24,32
    u8  descriptor;         // image descriptor bits (vh flip bits)
    
    // pixel data follows header
};

#pragma pack(pop)

bool SaveTGA(const char* filename, int width, int height, void* pdata)
{
	TGA_HEADER hdr;
	File::IOFile f(filename, "wb");
	if (!f)
		return false;

	_assert_(sizeof(TGA_HEADER) == 18 && sizeof(hdr) == 18);
    
	memset(&hdr, 0, sizeof(hdr));
	hdr.imagetype = 2;
	hdr.bits = 32;
	hdr.width = width;
	hdr.height = height;
	hdr.descriptor |= 8|(1<<5); // 8bit alpha, flip vertical

	f.WriteArray(&hdr, 1);
	f.WriteBytes(pdata, width * height * 4);

	return true;
}

bool SaveData(const char* filename, const char* data)
{
	std::ofstream f(filename, std::ios::binary);
	f << data;

	return true;
}
