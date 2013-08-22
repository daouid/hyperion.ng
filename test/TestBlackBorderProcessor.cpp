
// STL includes
#include <random>

// Utils includes
#include <utils/RgbImage.h>

#include "hyperion/BlackBorderProcessor.h"

RgbColor randomColor()
{
	const uint8_t randomRedValue   = uint8_t(rand() % (std::numeric_limits<uint8_t>::max() + 1));
	const uint8_t randomGreenValue = uint8_t(rand() % (std::numeric_limits<uint8_t>::max() + 1));
	const uint8_t randomBlueValue  = uint8_t(rand() % (std::numeric_limits<uint8_t>::max() + 1));

	return {randomRedValue, randomGreenValue, randomBlueValue};
}

RgbImage createImage(unsigned width, unsigned height, unsigned topBorder, unsigned leftBorder)
{
	RgbImage image(width, height);
	for (unsigned x=0; x<image.width(); ++x)
	{
		for (unsigned y=0; y<image.height(); ++y)
		{
			if (y < topBorder || x < leftBorder)
			{
				image(x,y) = RgbColor::BLACK;
			}
			else
			{
				image(x,y) = randomColor();
			}
		}
	}
	return image;
}

int main()
{
	unsigned unknownCnt = 600;
	unsigned borderCnt  = 50;

	BlackBorderProcessor processor;

	// Start with 'no border' detection
	RgbImage noBorderImage = createImage(64, 64, 0, 0);
	for (unsigned i=0; i<10; ++i)
	{
		bool newBorder = processor.process(noBorderImage);
		if (i == 0)
		{
			// Switch to 'no border' should immediate
			if (!newBorder)
			{
				std::cerr << "Failed to detect 'no border' when required" << std::endl;
				exit(EXIT_FAILURE);
			}
		}
		else
		{
			if (newBorder)
			{
				std::cerr << "Incorrectly detected new border, when there in none" << std::endl;
				exit(EXIT_FAILURE);
			}
		}
	}
	// Verify that the border is indeed
	if (processor.getCurrentBorder().type != BlackBorder::none)
	{
		std::cerr << "Incorrectlty identified 'no border' (" << processor.getCurrentBorder().type << " != " << BlackBorder::none << ")" << std::endl;
		exit(EXIT_FAILURE);
	}

	int borderSize = 12;
	RgbImage horzImage = createImage(64, 64, borderSize, 0);
	for (unsigned i=0; i<borderCnt*2; ++i)
	{
		bool newBorder = processor.process(horzImage);
		if (i == borderCnt)
		{
			if (!newBorder)
			{
				std::cerr << "Failed to detect 'horizontal border' when required after " << borderCnt << " images" << std::endl;
				exit(EXIT_FAILURE);
			}
		}
		else
		{
			if (newBorder)
			{
				std::cerr << "Incorrectly detected new border, when there in none" << std::endl;
				exit(EXIT_FAILURE);
			}
		}
	}
	if (processor.getCurrentBorder().type != BlackBorder::horizontal || processor.getCurrentBorder().size != borderSize)
	{
		std::cerr << "Incorrectlty found 'horizontal border' (" << processor.getCurrentBorder().type << " != " << BlackBorder::horizontal << ")" << std::endl;
		exit(EXIT_FAILURE);
	}

	// Switch back (in one shot) to no border
	if (!processor.process(noBorderImage) || (processor.getCurrentBorder().type != BlackBorder::none))
	{
		std::cerr << "Failed to switch back to 'no border' with one image" << std::endl;
		exit(EXIT_FAILURE);
	}

	RgbImage vertImage = createImage(64, 64, 0, borderSize);
	for (unsigned i=0; i<borderCnt*2; ++i)
	{
		bool newBorder = processor.process(vertImage);
		if (i == borderCnt)
		{
			if (!newBorder)
			{
				std::cerr << "Failed to detect 'vertical border' when required after " << borderCnt << " images" << std::endl;
				exit(EXIT_FAILURE);
			}
		}
		else
		{
			if (newBorder)
			{
				std::cerr << "Incorrectly detected new border, when there in none" << std::endl;
				exit(EXIT_FAILURE);
			}
		}
	}
	if (processor.getCurrentBorder().type != BlackBorder::vertical || processor.getCurrentBorder().size != borderSize)
	{
		std::cerr << "Incorrectlty found 'vertical border' (" << processor.getCurrentBorder().type << " != " << BlackBorder::horizontal << ")" << std::endl;
		exit(EXIT_FAILURE);
	}

	// Switch back (in one shot) to no border
	assert(processor.process(noBorderImage));
	assert(processor.getCurrentBorder().type == BlackBorder::none);

	return 0;
}