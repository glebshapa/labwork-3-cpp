#pragma once

#include <iostream>
#include <vector>

struct Color {
	uint8_t r{};
	uint8_t g{};
	uint8_t b{};

	Color() : r(0), g(0), b(0) {}

	Color(uint8_t r, uint8_t g, uint8_t b) : r(r), g(g), b(b) {}
};

struct Image {
	uint16_t width;
	uint16_t height;
	std::vector<Color> colors;

	Image(uint16_t width, uint16_t height) : width(width), height(height), colors(std::vector<Color>(width * height)) {}

	Color GetColor(uint16_t x, uint16_t y) const;

	void SetColor(const Color& color, uint16_t x, uint16_t y);

	void Export(const std::string& path) const;
};