#include "Sun.hpp"
#include "Map.hpp"

Sun::Sun(void) :
	Decor(),
	m_sizeCorner(),
	m_transparency(sf::Color::White),
	mf_angle(0.f)
{
}

Sun::~Sun(void)
{
}

void Sun::createOneSun(sf::Vector2f p_size, sf::Vector2f p_sizeCorner, sf::Vector2f p_origin, sf::Color p_color)
{
	sf::Vector2f upLeft(-p_size.x + p_sizeCorner.x, -p_size.y);
	sf::Vector2f upRight(p_size.x - p_sizeCorner.x, -p_size.y);
	sf::Vector2f cornerUpLeft(-p_size.x + p_sizeCorner.x, -p_size.y + p_sizeCorner.y);
	sf::Vector2f cornerUpRight(p_size.x - p_sizeCorner.x, -p_size.y + p_sizeCorner.y);
	sf::Vector2f upMidLeft(-p_size.x, -p_size.y + p_sizeCorner.y);
	sf::Vector2f upMidRight(p_size.x, -p_size.y + p_sizeCorner.y);
	sf::Vector2f downLeft(-p_size.x + p_sizeCorner.x, p_size.y);
	sf::Vector2f downRight(p_size.x - p_sizeCorner.x, p_size.y);
	sf::Vector2f cornerDownLeft(-p_size.x + p_sizeCorner.x, p_size.y - p_sizeCorner.y);
	sf::Vector2f cornerDownRight(p_size.x - p_sizeCorner.x, p_size.y - p_sizeCorner.y);
	sf::Vector2f downMidLeft(-p_size.x, p_size.y - p_sizeCorner.y);
	sf::Vector2f downMidRight(p_size.x, p_size.y - p_sizeCorner.y);

	createTriangle(upLeft, cornerUpLeft, upMidLeft, p_origin, p_color);
	createTriangle(upRight, cornerUpRight, upMidRight, p_origin, p_color);
	createTriangle(downLeft, cornerDownLeft, downMidLeft, p_origin, p_color);
	createTriangle(downRight, cornerDownRight, downMidRight, p_origin, p_color);

	createRectangle(upMidLeft, cornerUpLeft, cornerDownLeft, downMidLeft, p_origin, p_color);
	createRectangle(upMidRight, cornerUpRight, cornerDownRight, downMidRight, p_origin, p_color);
	createRectangle(upLeft, upRight, downRight, downLeft, p_origin, p_color);
}

void Sun::createSun(void)
{
	mn_countTriangle = 0;

	m_transparency.a = 110 - (110 * mf_timer / mf_maxTimer);
	float coef = mf_timer * mf_mouvement / 2.f;
	createOneSun(m_size * coef, m_sizeCorner * coef, m_origin, m_transparency);
	for (int i = m_biome->m_sun.mn_nb; i > 0; i--)
	{
		m_transparency.a = 110 - i;
		coef = (1.f + i / 4.f) * mf_mouvement;
		createOneSun(m_size * coef, m_sizeCorner * coef, m_origin, m_transparency);
	}
	createOneSun(m_size * mf_mouvement, m_sizeCorner * mf_mouvement, m_origin, m_transparency);
	createLightness();
}

void Sun::randomDecor(void)
{
	Decor::randomDecor();
	m_color = sf::Color(255, 255, 255);
	float size = randomRange(m_biome->m_sun.mn_minSizeX, m_biome->m_sun.mn_maxSizeX);
	m_size = sf::Vector2f(size, size);
	m_origin = sf::Vector2f(150, 150);
	m_sizeCorner = sf::Vector2f(m_size.x / 2.f, m_size.y / 2.f);
	m_transparency = sf::Color(m_color.r, m_color.g, m_color.b);

	// Init containers
	mn_maxTriangle = 10 * (m_biome->m_sun.mn_nb + 3);
	allocateVertex(mn_maxTriangle * 3u);
	createSun();
}

void Sun::createLightness(void)
{
	if (mf_angle > 0.f && mf_angle < 180.f)
	{
		sf::Vector2f upLeft(0.f, 0.f);
		sf::Vector2f upRight(1900.f, 0.f);
		sf::Vector2f downLeft(0.f, 1080.f);
		sf::Vector2f downRight(1900, 1080.f);
		sf::Vector2f origin(0.f, 0.f);
		sf::Color color(255, 128, 0);
		if (mf_angle > 0.f && mf_angle < 90.f)
			color.a = mf_angle;
		else
			color.a = 180.f - mf_angle;

		createRectangle(upLeft, upRight, downRight, downLeft, origin, color);
	}
}


void Sun::computeOrigin(float pf_deltatime)
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

void Sun::init(Biome * p_biome)
{
	Decor::init(p_biome);
	randomDecor();
	createSun();
}

void Sun::update(float pf_deltatime)
{
	Decor::update(pf_deltatime);

	computeOrigin(pf_deltatime);
	createSun();
}
