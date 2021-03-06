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

#include "BannerLoader.h"
#include "BannerLoaderWii.h"
#include "BannerLoaderGC.h"

#include "VolumeCreator.h"
#include "FileUtil.h"

// HyperIris: dunno if this suitable, may be need move.
#ifdef _WIN32
#include <Windows.h>
#else
#include <sys/param.h>
#include <iconv.h>
#include <errno.h>
#endif

#ifndef ICONV_CONST
#if defined __FreeBSD__ || __NetBSD__
#define ICONV_CONST const
#else
#define ICONV_CONST
#endif
#endif

namespace DiscIO
{
void IBannerLoader::CopyToStringAndCheck(std::string& _rDestination, const char* _src)
{
	static bool bValidChars[256];
	static bool bInitialized = false;

	if (!bInitialized)
	{
		for (int i = 0; i < 0x20; i++)
		{
			bValidChars[i] = false;
		}

		// generate valid chars
		for (int i = 0x20; i < 256; i++)
		{
			bValidChars[i] = true;
		}

		bValidChars[0x0a] = true;
		//bValidChars[0xa9] = true;
		//bValidChars[0xe9] = true;

		bInitialized = true;
	}

	char destBuffer[2048] = {0};
	char* dest = destBuffer;
	const char* src = _src;

	// copy the string and check for "unknown" characters
	while (*src != 0x00)
	{
		u8 c = *src;

		if (c == 0x0a){c = 0x20;}

		if (bValidChars[c] == false)
		{
			src++;
			continue;
		}

		*dest = c;
		dest++;
		src++;
	}

	// finalize the string
	*dest = 0x00;

	_rDestination = destBuffer;
}

bool IBannerLoader::CopyBeUnicodeToString( std::string& _rDestination, const u16* _src, int length )
{
	bool returnCode = false;
#ifdef WIN32
	if (_src)
	{
		u16* buffer = new u16[length];
		if (buffer)
		{
			memcpy(buffer, _src, sizeof(u16)*length);
			for (int i = 0; i < length; i++)
			{
				buffer[i] = swap16(buffer[i]);
			}
			
			u32 ansiNameSize = WideCharToMultiByte(932, 0, 
				(LPCWSTR)buffer, (int)wcslen((LPCWSTR)buffer),
				NULL, NULL, NULL, NULL);
			if (ansiNameSize > 0)
			{
				char* pAnsiStrBuffer = new char[ansiNameSize + 1];
				if (pAnsiStrBuffer)
				{
					memset(pAnsiStrBuffer, 0, (ansiNameSize + 1) * sizeof(char));
					if (WideCharToMultiByte(932, 0, 
						(LPCWSTR)buffer, (int)wcslen((LPCWSTR)buffer),
						pAnsiStrBuffer, ansiNameSize, NULL, NULL))
					{
						_rDestination = pAnsiStrBuffer;
						returnCode = true;
					}
					delete[] pAnsiStrBuffer;
				}
			}	
			delete[] buffer;
		}
	}
#else
	if (_src)
	{
		iconv_t conv_desc = iconv_open("UTF-8", "CP932");
		if (conv_desc == (iconv_t) -1)
		{
			// Initialization failure.
			if (errno == EINVAL)
			{
				ERROR_LOG(DISCIO, "Conversion from CP932 to UTF-8 is not supported.");
			}
			else
			{
				ERROR_LOG(DISCIO, "Iconv initialization failure: %s\n", strerror (errno));
			}
			return false;
		}

		char* src_buffer = new char[length];
		for (int i = 0; i < length; i++)
			src_buffer[i] = swap16(_src[i]);

		size_t inbytes = sizeof(char) * length;
		size_t outbytes = 2 * inbytes;
		char* utf8_buffer = new char[outbytes + 1];
		memset(utf8_buffer, 0, (outbytes + 1) * sizeof(char));

		// Save the buffer locations because iconv increments them
		char* utf8_buffer_start = utf8_buffer;
		char* src_buffer_start = src_buffer;

		size_t iconv_size = iconv(conv_desc,
			(ICONV_CONST char**)&src_buffer, &inbytes,
			&utf8_buffer, &outbytes);

		// Handle failures
		if (iconv_size == (size_t) -1)
		{
			ERROR_LOG(DISCIO, "iconv failed.");
			switch (errno) {
				case EILSEQ:
					ERROR_LOG(DISCIO, "Invalid multibyte sequence.");
					break;
				case EINVAL:
					ERROR_LOG(DISCIO, "Incomplete multibyte sequence.");
					break;
				case E2BIG:
					ERROR_LOG(DISCIO, "Insufficient space allocated for output buffer.");
					break;
				default:
					ERROR_LOG(DISCIO, "Error: %s.", strerror(errno));
			}
		}
		else
		{
			_rDestination = utf8_buffer_start;
			returnCode = true;
		}
		delete[] utf8_buffer_start;
		delete[] src_buffer_start;
		iconv_close(conv_desc);
	}
#endif
	return returnCode;
}

IBannerLoader* CreateBannerLoader(DiscIO::IFileSystem& _rFileSystem, DiscIO::IVolume *pVolume)
{
	if (IsVolumeWiiDisc(pVolume) || IsVolumeWadFile(pVolume))
	{
		return new CBannerLoaderWii(pVolume);
	}
	if (_rFileSystem.IsValid()) 
	{
		return new CBannerLoaderGC(_rFileSystem);
	}

	return NULL;
}

} // namespace
