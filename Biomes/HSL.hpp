#ifndef HSL_COLOR
#define HSL_COLOR

#include <SFML/Graphics/Color.hpp>
#include <algorithm>
#include <cmath>

struct HSL
{
	float Hue;
	float Saturation;
	float Luminance;

	HSL();
	HSL(int H, int S, int L);

	sf::Color TurnToRGB();

	static constexpr float Epsilon = 0.00000000000001;

	private:
	float HueToRGB(float arg1, float arg2, float H);
};

HSL TurnToHSL(const sf::Color& C);

#endif // HSL_COLOR
