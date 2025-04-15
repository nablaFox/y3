#pragma once

namespace etna {

struct Color {
	float r, g, b, a;

	Color setAlpha(float alpha) const { return {r, g, b, alpha}; }

	static Color RGBA(float r, float g, float b, float a) {
		return {r / 255.f, g / 255.f, b / 255.f, a / 255.f};
	}

	static Color RGB(float r, float g, float b) {
		return {r / 255.f, g / 255.f, b / 255.f, 1};
	}

	Color operator*(float brightness) const {
		return {r * brightness, g * brightness, b * brightness, a};
	}
};

const inline Color WHITE = Color{.r = 1, .g = 1, .b = 1, .a = 1};

const inline Color BLACK = Color{.r = 0, .g = 0, .b = 0, .a = 1};

const inline Color RED = Color::RGB(231, 47, 69);

const inline Color GREEN = Color::RGB(125, 215, 93);

const inline Color BLUE = Color::RGB(110, 150, 252);

const inline Color PURPLE = Color::RGB(167, 152, 220);

const inline Color INVISIBLE = Color{.r = 0, .g = 0, .b = 0, .a = 0};

const inline Color CELESTE = Color::RGB(135, 206, 250);

const inline Color YELLOW = Color::RGB(255, 255, 0);

};	// namespace etna
