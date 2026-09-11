#include "ui_helpers.h"

namespace UiHelpers {

PNG png;
PngTarget *activePngTarget = nullptr;
int16_t pngX = 0;
int16_t pngY = 0;
uint16_t pngTargetWidth = 0;
uint16_t pngTargetHeight = 0;

int pngDraw(PNGDRAW *line)
{
	if (activePngTarget == nullptr) {
		return 0;
	}

	uint16_t lineBuffer[PNG_LINE_BUFFER_WIDTH];
	uint16_t scaledLine[PNG_LINE_BUFFER_WIDTH];
	png.getLineAsRGB565(line, lineBuffer, PNG_RGB565_BIG_ENDIAN, 0xffffffff);

	const uint16_t sourceWidth = line->iWidth;
	const uint16_t sourceHeight = png.getHeight();
	const uint16_t firstTargetY = (line->y * pngTargetHeight) / sourceHeight;
	const uint16_t nextTargetY = ((line->y + 1) * pngTargetHeight) / sourceHeight;
	const uint16_t width = min(pngTargetWidth, PNG_LINE_BUFFER_WIDTH);

	for (uint16_t targetX = 0; targetX < width; ++targetX) {
		const uint16_t sourceX = (targetX * sourceWidth) / pngTargetWidth;
		scaledLine[targetX] = lineBuffer[sourceX];
	}

	for (uint16_t targetRow = firstTargetY; targetRow < nextTargetY; ++targetRow) {
		activePngTarget->drawLine(pngX, pngY + targetRow, width, scaledLine);
	}

	return 1;
}

bool drawPng(TFT_eSprite &target, const byte *data, size_t dataSize,
					int16_t centerX, int16_t centerY, float scale)
{
	PngTarget renderTarget(target);
	activePngTarget = &renderTarget;

	if (png.openFLASH(const_cast<uint8_t *>(data), dataSize, pngDraw) != PNG_SUCCESS) {
		activePngTarget = nullptr;
		return false;
	}

	pngTargetWidth = max<uint16_t>(1, png.getWidth() * scale);
	pngTargetHeight = max<uint16_t>(1, png.getHeight() * scale);
	pngX = centerX - (pngTargetWidth / 2);
	pngY = centerY - (pngTargetHeight / 2);
	const bool decoded = png.decode(nullptr, 0) == PNG_SUCCESS;
	activePngTarget = nullptr;
	return decoded;
}

void drawProgressArc(TFT_eSprite &target, int16_t centerX,
							int16_t centerY, int16_t radius, float progress)
{
	target.drawArc(centerX, centerY, radius,
				   radius - DisplayConfig::RING_WIDTH, 0, 360,
				   DisplayConfig::TRACK_COLOR, DisplayConfig::BACKGROUND, true);

	if (progress > 0.0f) {
		target.drawArc(centerX, centerY, radius,
					   radius - DisplayConfig::RING_WIDTH, 0,
					   360.0f * progress,
					   DisplayConfig::PROGRESS_COLOR, DisplayConfig::BACKGROUND, true);
	}
}

FontSelection getFont(uint8_t size)
{
	const uint8_t baseSize = size <= 10 ? 9 :
							 size <= 15 ? 12 :
							 size <= 27 ? 18 : 24;
	const uint8_t scale = max<uint8_t>(1, (size + baseSize - 1) / baseSize);

	switch (baseSize) {
	case 9:
		return {&FreeSansBold9pt7b, scale};
	case 12:
		return {&FreeSansBold12pt7b, scale};
	case 24:
		return {&FreeSansBold24pt7b, scale};
	default:
		return {&FreeSansBold18pt7b, scale};
	}
}

void drawText(TFT_eSprite &target, const char *text, int16_t x, int16_t y,
				  uint8_t size, uint16_t color, uint16_t background)
{
	target.setTextDatum(MC_DATUM);
	target.setTextColor(color, background);
	const FontSelection font = getFont(size);
	target.setFreeFont(font.font);
	target.setTextSize(font.scale);
	target.drawString(text, x, y);
	target.setTextSize(1);
	target.setTextFont(1);
}

} // namespace UiHelpers
