#include "ui_helpers.h"

namespace UiHelpers
{

	void drawProgressArc(TFT_eSprite &target,
						 int16_t centerX,
						 int16_t centerY,
						 int16_t radius,
						 float progress,
						 uint16_t color)
	{
		target.drawArc(centerX, centerY, radius,
					   radius - DisplayConfig::RING_WIDTH,
					   0, 360,
					   DisplayConfig::TRACK_COLOR,
					   DisplayConfig::BACKGROUND, true);

		if (progress <= 0.0f)
		{
			return;
		}

		progress = constrain(progress, 0.0f, 1.0f);

		float sweep = 360.0f * progress;
		float end = 180.0f - sweep;

		if (end >= 0.0f)
		{
			target.drawArc(centerX, centerY, radius,
						   radius - DisplayConfig::RING_WIDTH,
						   end,
						   180,
						   color,
						   DisplayConfig::BACKGROUND, true);
		}
		else
		{
			// 180° -> 0°
			target.drawArc(centerX, centerY, radius,
						   radius - DisplayConfig::RING_WIDTH,
						   0,
						   180,
						   color,
						   DisplayConfig::BACKGROUND, true);

			// Wrap around: 360° -> remaining angle
			target.drawArc(centerX, centerY, radius,
						   radius - DisplayConfig::RING_WIDTH,
						   360.0f + end,
						   360,
						   color,
						   DisplayConfig::BACKGROUND, true);
		}
	}

	FontSelection getFont(uint8_t size)
	{
		const uint8_t baseSize = size <= 10 ? 9 : size <= 15 ? 12
											  : size <= 27	 ? 18
															 : 24;
		const uint8_t scale = max<uint8_t>(1, (size + baseSize - 1) / baseSize);

		switch (baseSize)
		{
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

	void shortenText(char *str, size_t maxLength)
	{
		size_t len = strlen(str);

		if (len > maxLength)
		{
			if (maxLength >= 3)
			{
				str[maxLength - 3] = '.';
				str[maxLength - 2] = '.';
				str[maxLength - 1] = '.';
				str[maxLength] = '\0';
			}
			else
			{
				str[maxLength] = '\0';
			}
		}
	}

} // namespace UiHelpers
