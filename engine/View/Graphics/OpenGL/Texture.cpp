#include "Texture.h"
Texture::Texture() {}

Texture::Texture(const char* fileName) 
{
	filePath = fileName;
	stbi_set_flip_vertically_on_load(true);
	imageData = stbi_load(fileName,&width,&height,&numColorChannels,0);
	GenTexture(imageData,width,height,numColorChannels);
	
}

Texture::Texture(unsigned char* imgData, int w, int h,int c) {
	GenTexture(imgData,w,h,c);
}

Texture::~Texture() {
	glDeleteTextures(1,&ID);
}

void Texture::Bind(GLuint tUnit) {
	glActiveTexture(tUnit);
	glBindTexture(GL_TEXTURE_2D,ID);
	glEnable(GL_TEXTURE_2D);
}

unsigned char Texture::GetPixelValue(float x, float y, int index) {
	if (index > numColorChannels - 1) {
		return 0;
	}

	int x0 = static_cast<int>(x);
	int y0 = static_cast<int>(y);
	int x1 = x0 + 1;
	int y1 = y0 + 1;

	float dx = x - x0;
	float dy = y - y0;

	unsigned char p00 = GetPixelValueClamped(x0, y0, index);
	unsigned char p01 = GetPixelValueClamped(x0, y1, index);
	unsigned char p10 = GetPixelValueClamped(x1, y0, index);
	unsigned char p11 = GetPixelValueClamped(x1, y1, index);

	float interpolatedValue = (1 - dx) * (1 - dy) * p00 +
		dx * (1 - dy) * p10 +
		(1 - dx) * dy * p01 +
		dx * dy * p11;

	return static_cast<unsigned char>(interpolatedValue);
}

unsigned char Texture::GetPixelValueClamped(int x, int y, int index) {
	x = std::clamp(x, 0, width - 1);
	y = std::clamp(y, 0, height - 1);
	return imageData[numColorChannels * (y * width + x) + index];
}

int Texture::GetChannelCount() {
	return numColorChannels;
}

int Texture::GetWidth() { return width; }

int Texture::GetHeight() { return height; }

std::string Texture::GetPath() { return filePath; }

void Texture::GenTexture(unsigned char* imgData, int w, int h, int c) {

	if (!imgData)
		std::cout << "could not load image data: " <<  filePath <<std::endl;

	width = w;
	height = h;
	numColorChannels = c;

	glGenTextures(1, &ID);

	glBindTexture(GL_TEXTURE_2D, ID);//bind to change settings.

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);//s for x axis
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);//t for y axis

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);//when image is squished
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);//when image is enlarged


	unsigned int format = GL_RED;
	if (numColorChannels == 1) {
		format = GL_RED;
	}
	else if (numColorChannels == 2) {
		format = GL_RG;
	}
	else if (numColorChannels == 3) {
		format = GL_RGB;
	}
	else if (numColorChannels == 4) {
		format = GL_RGBA;
	}

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, format, GL_UNSIGNED_BYTE, imgData);

	imageData = imgData;
	glGenerateMipmap(GL_TEXTURE_2D);

}