#pragma once

#include <iostream>
#include <vector>

struct Image {
	uint16_t width;
	uint16_t height;
	std::vector<uint8_t> colors;

	Image(uint16_t width, uint16_t height) : width(width), height(height), colors(std::vector<uint8_t>(width * height)) {}

	uint8_t GetColor(uint16_t x, uint16_t y) const;

	void SetColor(const uint8_t& color, uint16_t x, uint16_t y);

	void Export(const std::string& path) const;
};