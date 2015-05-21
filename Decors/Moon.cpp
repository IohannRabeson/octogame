#include "Moon.hpp"
#include "Map.hpp"

Moon::Moon() :
	Decor(),
	mf_angle(180.f)
{
}

Moon::~Moon()
{
}

void Moon::createOneMoon(sf::Vector2f p_size, sf::Vector2f p_origin, sf::Color p_color)
{
	float unit = p_size.x / 3.f;

	sf::Vector2f upLeft(-unit * 2.f, -p_size.y);
	sf::Vector2f upRight(-unit, -p_size.y);
	sf::Vector2f quarterUpLeft(-p_size.x, -p_size.y + unit);
	sf::Vector2f quarterUpRight(-unit * 2.f, -p_size.y + unit);
	sf::Vector2f upMidLeft(-unit, -unit);
	sf::Vector2f upMidRight(unit, -unit);
	sf::Vector2f downMidLeft(-unit, unit);
	sf::Vector2f downMidRight(unit, unit);
	sf::Vector2f quarterDownLeft(-p_size.x, p_size.y - unit);
	sf::Vector2f quarterDownRight(-unit * 2.f, p_size.y - unit);
	sf::Vector2f downLeft(-unit * 2.f, p_size.y);
	sf::Vector2f downRight(-unit, p_size.y);

	createRectangle(upLeft, upRight, quarterUpRight, quarterUpLeft, p_origin, p_color);
	createRectangle(upRight, upMidRight, upMidLeft, quarterUpRight, p_origin, p_color);
	createRectangle(upMidRight, downMidRight, downMidLeft, upMidLeft, p_origin, p_color);
	createRectangle(downLeft, downRight, quarterDownRight, quarterDownLeft, p_origin, p_color);
	createRectangle(downRight, downMidRight, downMidLeft, quarterDownRight, p_origin, p_color);
}

void Moon::createMoon(void)
{
	mn_countTriangle = 0;

	/*
	m_transparency.a = 110 - (110 * mf_timer / mf_maxTimer);
	float coef = mf_timer * mf_mouvement / 2.f;
	createOneMoon(m_size * coef, m_origin, m_transparency);
	for (int i = m_biome->m_sun.mn_nb; i > 0; i--)
	{
		coef = (1.f + i / 4.f) * mf_mouvement;
		createOneMoon(m_size * coef, m_origin, m_transparency);
	}
	*/
	m_transparency.a = 180;
	createOneMoon(m_size * mf_mouvement, m_origin, m_transparency);
	createDarkness();
}

void Moon::randomDecor(void)
{
	Decor::randomDecor();
	m_color = sf::Color(255, 255, 255);
	float size = randomRange(m_biome->m_sun.mn_minSizeX, m_biome->m_sun.mn_maxSizeX);
	m_size = sf::Vector2f(size, size);
	m_origin = sf::Vector2f(300, 150);
	m_transparency = sf::Color(m_color.r, m_color.g, m_color.b);

	// Init containers
	mn_maxTriangle = 10 * (m_biome->m_sun.mn_nb + 2);
	allocateVertex(mn_maxTriangle * 3u);
	createMoon();
}

void Moon::createDarkness(void)
{
	if (mf_angle > 0.f && mf_angle < 180.f)
	{
		sf::Vector2f upLeft(0.f, 0.f);
		sf::Vector2f upRight(1900.f, 0.f);
		sf::Vector2f downLeft(0.f, 1080.f);
		sf::Vector2f downRight(1900, 1080.f);
		sf::Vector2f origin(0.f, 0.f);
		sf::Color color(0, 102, 204);
		if (mf_angle > 0.f && mf_angle < 90.f)
			color.a = mf_angle;
		else
			color.a = 180.f - mf_angle;

		createRectangle(upLeft, upRight, downRight, downLeft, origin, color);
	}
}

void Moon::computeOrigin(float pf_deltatime)
{
	m_origin = sf::Vector2f(-1900 / 2.f, -1080 / 2.f);
	mf_angle += pf_deltatime * 10;
	if (mf_angle >= 360.f)
		mf_angle = 0.f;
	float cosA = cos(mf_angle * PI / 180.f);
	float sinA = sin(mf_angle * PI / 180.f);
	rotateVec(&m_origin, cosA, sinA);
	m_origin.x += 1900 / 2.f;
	m_origin.y += 1080;
}

void Moon::init(Biome * p_biome)
{
	Decor::init(p_biome);
	randomDecor();
	createMoon();
}

void Moon::update(float pf_deltatime)
{
	Decor::update(pf_deltatime);

	computeOrigin(pf_deltatime);
	createMoon();
}
