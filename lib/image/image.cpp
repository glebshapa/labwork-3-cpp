#include "image.h"

#include <iostream>
#include <fstream>
#include <cstring>

const uint16_t FILE_HEADER_SIZE = 14;
const uint16_t INFORMATION_HEADER_SIZE = 40;

uint8_t Image::GetColor(uint16_t x, uint16_t y) const {
	return colors[y * width + x];
}

void Image::SetColor(const uint8_t& color, uint16_t x, uint16_t y) {
	colors[y * width + x] = color;
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
	fileHeader[10] = FILE_HEADER_SIZE + INFORMATION_HEADER_SIZE + 20;
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
	informationHeader[14] = 4;
}

void GenerateHeaderColorsCount(unsigned char& informationHeaderLink) {
	unsigned char* informationHeader = &informationHeaderLink;
	informationHeader[32] = 5;
	informationHeader[36] = 5;
}

void Image::Export(const std::string& path) const {
	std::ofstream f;
	f.open(path, std::ios::out | std::ios::binary);

	if (!f.is_open()) {
		std::cout << "File could not be opened\n";
		return;
	}

	const int padding_width = ((width + 7) / 8) * 4;

	const uint32_t fileSize = FILE_HEADER_SIZE + INFORMATION_HEADER_SIZE + height * padding_width;

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
	GenerateHeaderColorsCount(*informationHeader);

	unsigned char palette[20] = {255, 255, 255, 0,
															 0, 255, 0, 0,
															 255, 0, 150, 0,
															 0, 255, 255, 0,
															 0, 0, 0, 0};

	f.write(reinterpret_cast<char*>(fileHeader), FILE_HEADER_SIZE);
	f.write(reinterpret_cast<char*>(informationHeader), INFORMATION_HEADER_SIZE);
	f.write(reinterpret_cast<char*>(palette), 20);

	for (int y = 0; y < height; y++) {
		for (int x = 0; x < padding_width; x++) {
			uint8_t pxl = GetColor(2 * x, y);
			pxl = pxl << 4;
			pxl += GetColor(2 * x + 1, y);
			unsigned char color[] = { pxl };
			f.write(reinterpret_cast<char*>(color), 1);
		}
	}
	f.close();
}