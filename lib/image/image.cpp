#include "image.h"

#include <iostream>
#include <fstream>
#include <cstring>

const uint16_t FILE_HEADER_SIZE = 14;
const uint16_t INFORMATION_HEADER_SIZE = 40;

Color Image::GetColor(uint16_t x, uint16_t y) const {
	return colors[y * width + x];
}

void Image::SetColor(const Color& color, uint16_t x, uint16_t y) {
	colors[y * width + x].r = color.r;
	colors[y * width + x].g = color.g;
	colors[y * width + x].b = color.b;
}

void GenerateFileType(unsigned char& fileHeaderLink) {
	unsigned char* fileHeader = &fileHeaderLink;
	fileHeader[0] = 'B';
	fileHeader[1] = 'M';
}

void GenerateFileSize(unsigned char& fileHeaderLink, const uint32_t fileSize) {
	unsigned char* fileHeader = &fileHeaderLink;
	fileHeader[2] = fileSize;
	fileHeader[3] = fileSize >> 8;
	fileHeader[4] = fileSize >> 16;
	fileHeader[5] = fileSize >> 24;
}

void GenerateFilePixelDataOffset(unsigned char& fileHeaderLink) {
	unsigned char* fileHeader = &fileHeaderLink;
	fileHeader[10] = FILE_HEADER_SIZE + INFORMATION_HEADER_SIZE;
}

void GenerateHeaderInformationSize(unsigned char& informationHeaderLink) {
	unsigned char* informationHeader = &informationHeaderLink;
	informationHeader[0] = INFORMATION_HEADER_SIZE;
}

void GenerateHeaderInformationImageSize(unsigned char& informationHeaderLink, uint16_t width, uint16_t height) {
	unsigned char* informationHeader = &informationHeaderLink;
	informationHeader[4] = width;
	informationHeader[5] = width >> 8;
	informationHeader[6] = width >> 16;
	informationHeader[7] = width >> 24;
	informationHeader[8] = height;
	informationHeader[9] = height >> 8;
	informationHeader[10] = height >> 16;
	informationHeader[11] = height >> 24;
}

void GenerateHeaderPlanes(unsigned char& informationHeaderLink) {
	unsigned char* informationHeader = &informationHeaderLink;
	informationHeader[12] = 1;
}

void GenerateHeaderBitPerPixel(unsigned char& informationHeaderLink) {
	unsigned char* informationHeader = &informationHeaderLink;
	informationHeader[14] = 24;
}

void Image::Export(const std::string& path) const {
	std::ofstream f;
	f.open(path, std::ios::out | std::ios::binary);

	if (!f.is_open()) {
		std::cout << "File could not be opened\n";
		return;
	}

	unsigned char bmpPad[3] = { 0, 0, 0 };
	const uint32_t paddingAmount = ((4 - (width * 3) % 4) % 4);
	const uint32_t fileSize = FILE_HEADER_SIZE + INFORMATION_HEADER_SIZE + width * height * 3 + paddingAmount * width;
	
	unsigned char fileHeader[FILE_HEADER_SIZE];
	std::memset(fileHeader, 0, FILE_HEADER_SIZE);

	GenerateFileType(*fileHeader);
	GenerateFileSize(*fileHeader, fileSize);
	GenerateFilePixelDataOffset(*fileHeader);

	unsigned char informationHeader[INFORMATION_HEADER_SIZE];
	std::memset(informationHeader, 0, INFORMATION_HEADER_SIZE);

	GenerateHeaderInformationSize(*informationHeader);
	GenerateHeaderInformationImageSize(*informationHeader, width, height);
	GenerateHeaderPlanes(*informationHeader);
	GenerateHeaderBitPerPixel(*informationHeader);

	f.write(reinterpret_cast<char*>(fileHeader), FILE_HEADER_SIZE);
	f.write(reinterpret_cast<char*>(informationHeader), INFORMATION_HEADER_SIZE);

	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			unsigned char r = static_cast<unsigned char>(GetColor(x, y).r);
			unsigned char g = static_cast<unsigned char>(GetColor(x, y).g);
			unsigned char b = static_cast<unsigned char>(GetColor(x, y).b);

			unsigned char color[] = { b, g, r };

			f.write(reinterpret_cast<char*>(color), 3);
		}
		f.write(reinterpret_cast<char*>(bmpPad), paddingAmount);
	}
	f.close();
}