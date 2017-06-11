#include <iostream>
#include <fstream>
#include "Bitmap.h"

bool bitmap::read(const char* filename)
{
	std::ifstream input(filename, std::ios::in | std::ifstream::binary);
	input.read((char*)&fh, sizeof(bitmapfileheader));
	input.read((char*)&ih, sizeof(bitmapinfoheader));
	if (fh.bfType != bmcode)
	{
		std::cout << "Invalid map file type: " << filename << std::endl;
		return false;
	}
	if (ih.biBitCount != colorbit)
	{
		std::cout << "Invalie bitmap type. Only 256-color-bitmap supported." << std::endl;
		return false;
	}

	std::vector<unsigned char> buffer(ih.biSizeImage);

	input.seekg(fh.bfOffBits, input.beg);
	input.read((char*)&buffer[0], buffer.size());

	pixels.resize(ih.biHeight * ih.biWidth);

	int bi = 0;
	int padding = ((ih.biWidth + 3) & (~3)) - ih.biWidth;
	for (int h = 0; h < ih.biHeight; ++h)
	{
		for (int w = 0; w < ih.biWidth; ++w)
		{
			set(h, w, buffer[bi++]);
		}
		bi += padding;
	}

	return true;
}

const unsigned char bitmap::get(int h, int w) const
{
	int index = h * height() + w;
	return pixels[index];
}

void bitmap::set(int h, int w, unsigned char p)
{
	int index = (height() - h - 1) * height() + w;
	pixels[index] = p;
}
