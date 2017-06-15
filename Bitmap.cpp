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

	mid.resize(fh.bfOffBits - sizeof(bitmapfileheader) - sizeof(bitmapinfoheader));
	input.read((char*)&mid[0], mid.size());
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

bool bitmap::write(const char* filename)
{
	std::ofstream output(filename, std::ios::out | std::ofstream::binary);
	output.write((char*)&fh, sizeof(bitmapfileheader));
	output.write((char*)&ih, sizeof(bitmapinfoheader));
	output.write((char*)&mid[0], mid.size());

	std::vector<unsigned char> buffer(ih.biSizeImage);

	int bi = 0;
	int padding = ((ih.biWidth + 3) & (~3)) - ih.biWidth;
	for (int h = 0; h < ih.biHeight; ++h)
	{
		for (int w = 0; w < ih.biWidth; ++w)
		{
			buffer[bi++] = (unsigned char)get(h, w);
		}
		for (int w = 0; w < padding; ++w)
		{
			buffer[bi++] = 0;
		}
	}

	output.write((char*)&buffer[0], buffer.size());

	return true;
}

bitmap::ecolor bitmap::get(int h, int w) const
{
	int index = h * height() + w;
	return bitmap::ecolor(pixels[index]);
}

void bitmap::set(int h, int w, unsigned char p)
{
	int index = (height() - h - 1) * height() + w;
	pixels[index] = p;
}
