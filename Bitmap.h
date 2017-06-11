#pragma once
#pragma pack(2)
#include <vector>

// 256color-bitmap
class bitmap
{
public:
	const int bmcode = 0x4D42;
	const int colorbit = 8;
	enum ecolor
	{
		black = 0x00,
		red = 0x4F,
		blue = 0xD2,
		white = 0xFF
	};

	bool read(const char* filename);

	const unsigned char get(int h, int w) const;
	const int width() const { return ih.biWidth; }
	const int height() const { return ih.biHeight; }
	const std::vector<unsigned char>& data() const { return pixels; }

private:
	struct bitmapfileheader				/**** BMP file header structure ****/
	{
		unsigned short bfType;			/* Magic number for file */
		unsigned int   bfSize;			/* Size of file */
		unsigned short bfReserved1;		/* Reserved */
		unsigned short bfReserved2;		/* ... */
		unsigned int   bfOffBits;		/* Offset to bitmap data */
	};

	struct bitmapinfoheader				/**** BMP file info structure ****/
	{
		unsigned int   biSize;			/* Size of info header */
		int            biWidth;			/* Width of image */
		int            biHeight;		/* Height of image */
		unsigned short biPlanes;		/* Number of color planes */
		unsigned short biBitCount;		/* Number of bits per pixel */
		unsigned int   biCompression;	/* Type of compression to use */
		unsigned int   biSizeImage;		/* Size of image data */
		int            biXPelsPerMeter;	/* X pixels per meter */
		int            biYPelsPerMeter;	/* Y pixels per meter */
		unsigned int   biClrUsed;		/* Number of colors used */
		unsigned int   biClrImportant;	/* Number of important colors */
	};

	void bitmap::set(int h, int w, unsigned char p);

private:
	bitmapfileheader fh;
	bitmapinfoheader ih;
	std::vector<unsigned char> pixels;
};

#pragma pack()