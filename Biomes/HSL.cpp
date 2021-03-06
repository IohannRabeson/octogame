#include "HSL.hpp"
//https://github.com/SFML/SFML/wiki/Source%3A-HSL-Color

HSL::HSL() :
	Hue(0),
	Saturation(0),
	Luminance(0)
{
}

HSL::HSL(int H, int S, int L)
{
	///Range control for Hue.
	if (H <= 360 && H >= 0)
		Hue = H;
	else
	{
		if (H > 360)
			Hue = H % 360;
		else if (H < 0 && H > -360)
			Hue = -H;
		else if (H < 0 && H < -360)
			Hue = -(H % 360);
	}

	///Range control for Saturation.
	if (S <= 100 && S >= 0)
		Saturation = S;
	else
	{
		if (S > 100)
			Saturation = S % 100;
		else if (S < 0 && S > -100)
			Saturation = -S;
		else if (S < 0 && S < -100)
			Saturation = -(S % 100);
	}

	///Range control for Luminance
	if (L <= 100 && L >= 0)
		Luminance = L;
	else
	{
		if (L > 100)
			Luminance = L % 100;
		if (L < 0 && L > -100)
			Luminance = -L;
		if (L < 0 && L < -100)
			Luminance = -(L % 100);
	}
}

float HSL::HueToRGB(float arg1, float arg2, float H)
{
	if (H < 0)
		H += 1;
	if (H > 1)
		H -= 1;
	if ((6 * H) < 1)
		return (arg1 + (arg2 - arg1) * 6 * H);
	if ((2 * H) < 1)
		return arg2;
	if ((3 * H) < 2)
		return (arg1 + (arg2 - arg1) * ((2.f / 3.f) - H) * 6);
	return arg1;
}


sf::Color HSL::TurnToRGB()
{
	if (Saturation > 100.f)
		Saturation = 100.f;
	else if (Saturation < 0.f)
		Saturation = 0.f;
	if (Luminance > 100.f)
		Luminance = 100.f;
	else if (Luminance < 0.f)
		Luminance = 0.f;
	if (Hue > 360.f)
		Hue = static_cast<int>(Hue) % 360;
	else if (Hue < 0.f)
		Hue += static_cast<int>(Hue) % 360 + 360.f;

	///Reconvert to range [0,1]
	float H = Hue / 360.f;
	float S = Saturation / 100.f;
	float L = Luminance / 100.f;

	if (S <= Epsilon)
	{
		sf::Color C(L * 255, L * 255, L * 255);
		return C;
	}
	else
	{
		float arg1, arg2;

		if (L < 0.5)
			arg2 = L * (1 + S);
		else
			arg2 = (L + S) - (S * L);
		arg1 = 2 * L - arg2;

		sf::Uint8 r = (255 * HueToRGB(arg1, arg2, (H + 1.f / 3.f)));
		sf::Uint8 g = (255 * HueToRGB(arg1, arg2, H));
		sf::Uint8 b = (255 * HueToRGB(arg1, arg2, (H - 1.f / 3.f)));
		sf::Color C(r,g,b);
		return C;
	}

}

HSL TurnToHSL(const sf::Color& C)
{
	///Trivial cases.
	if (C == sf::Color::White)
		return HSL(0, 0, 100);

	if (C == sf::Color::Black)
		return HSL(0, 0, 0);

	if (C == sf::Color::Red)
		return HSL(0, 100, 50);

	if (C == sf::Color::Yellow)
		return HSL(60, 100, 50);

	if (C == sf::Color::Green)
		return HSL(120, 100, 50);

	if (C == sf::Color::Cyan)
	{ return HSL(180, 100, 50); }

	if (C == sf::Color::Blue)
		return HSL(240, 100, 50);

	if (C == sf::Color::Cyan)
		return HSL(300, 100, 50);

	float R, G, B;
	R = C.r / 255.f;
	G = C.g / 255.f;
	B = C.b / 255.f;
	///Casos no triviales.
	float max, min, l, s = 0.f;

	///Maximos
	max = std::max(std::max(R,G),B);

	///Minimos
	min = std::min(std::min(R,G),B);


	HSL A;
	l = ((max + min) / 2.f);

	if (max - min <= HSL::Epsilon)
	{
		A.Hue = 0;
		A.Saturation = 0;
	}
	else
	{
		float diff = max - min;

		if (A.Luminance < 0.5)
			s = diff/(max + min);
		else
			s = diff/(2 - max - min);

		float diffR = ((( max - R) * 60) + (diff / 2.f)) / diff;
		float diffG = ((( max - G) * 60) + (diff / 2.f)) / diff;
		float diffB = ((( max - B) * 60) + (diff / 2.f)) / diff;


		if (max - R <= HSL::Epsilon)
			A.Hue = diffB - diffG;
		else if (max - G <= HSL::Epsilon)
			A.Hue = (1 * 360) / 3.f + (diffR - diffB);
		else if (max - B <= HSL::Epsilon)
			A.Hue = (2 * 360) / 3.f + (diffG - diffR);

		if (A.Hue <= 0 || A.Hue >= 360)
		{
			float r = fmod(A.Hue, 360);
			(void)r;
		}

		s *= 100;
	}

	l *= 100;
	A.Saturation = s;
	A.Luminance = l;
	return A;
}
