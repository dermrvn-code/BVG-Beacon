#pragma once

#include <Arduino.h>
#include <PNGdec.h>
#include <TFT_eSPI.h>
#include "Display_Config.h"

namespace UiHelpers
{

	constexpr uint16_t PNG_LINE_BUFFER_WIDTH = 256;

	struct PngTarget
	{
		TFT_eSprite *sprite = nullptr;
		TFT_eSPI *display = nullptr;

		PngTarget(TFT_eSprite &target) : sprite(&target) {}
		PngTarget(TFT_eSPI &target) : display(&target) {}

		void drawLine(int16_t x, int16_t y, uint16_t width,
					  const uint16_t *pixels)
		{
			if (sprite != nullptr)
			{
				sprite->pushImage(x, y, width, 1, pixels);
			}
			else
			{
				display->pushImage(x, y, width, 1, pixels);
			}
		}
	};

	extern PNG png;
	extern PngTarget *activePngTarget;
	extern int16_t pngX;
	extern int16_t pngY;
	extern uint16_t pngTargetWidth;
	extern uint16_t pngTargetHeight;

	int pngDraw(PNGDRAW *line);

	bool drawPng(TFT_eSprite &target, const byte *data, size_t dataSize,
				 int16_t centerX, int16_t centerY, float scale);

	void drawProgressArc(TFT_eSprite &target, int16_t centerX,
						 int16_t centerY, int16_t radius, float progress, uint16_t color);

	struct FontSelection
	{
		const GFXfont *font;
		uint8_t scale;
	};

	FontSelection getFont(uint8_t size);

	void drawText(TFT_eSprite &target, const char *text, int16_t x, int16_t y,
				  uint8_t size,
				  uint16_t color,
				  uint16_t background = DisplayConfig::BACKGROUND);

	void shortenText(char *str, size_t maxLength = 20);

} // namespace UiHelpers

using UiHelpers::drawPng;
using UiHelpers::drawProgressArc;
using UiHelpers::drawText;
using UiHelpers::shortenText;
