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
	int width;
	int height;
	std::vector<Color> colors;

	Image(int width, int height) : width(width), height(height), colors(std::vector<Color>(width * height)) {}

	Color GetColor(int x, int y) const;

	void SetColor(const Color& color, int x, int y);

	void Export(const char* path) const;
};