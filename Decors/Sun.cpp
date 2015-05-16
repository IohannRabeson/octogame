#include "Sun.hpp"
#include "Map.hpp"

Sun::Sun() :
	Decor()
{
}

Sun::~Sun()
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
	sf::Vector2f sizeCorner(m_size.x / 2.f, m_size.y / 2.f);
	sf::Color transparency(m_color.r, m_color.g, m_color.b, 10);
	for (int i = m_biome->m_sun.mn_nb; i > 0; i--)
	{
		float coef = 1.f + i / 5.f;
		createOneSun(sf::Vector2f(m_size.x * coef * mf_mouvement, m_size.y * coef * mf_mouvement), sf::Vector2f(sizeCorner.x * coef * mf_mouvement, sizeCorner.y * coef * mf_mouvement), m_origin, transparency);
		transparency.a = 110 - i * 10;
	}
	createOneSun(sf::Vector2f(m_size.x * mf_mouvement, m_size.y * mf_mouvement), sf::Vector2f(sizeCorner.x * mf_mouvement, sizeCorner.y * mf_mouvement), m_origin, m_color);
}

void Sun::randomDecor(void)
{
	Decor::randomDecor();
	m_color = sf::Color(255, 255, 255);
	m_size = sf::Vector2f(randomRange(m_biome->m_sun.mn_minSizeX, m_biome->m_sun.mn_maxSizeX), randomRange(m_biome->m_sun.mn_minSizeY, m_biome->m_sun.mn_maxSizeY));
	m_origin = sf::Vector2f(150, 150);

	// Init containers
	mn_maxTriangle = 10 * (m_biome->m_sun.mn_nb + 1);
	m_triangle.reset(new sf::Vertex[mn_maxTriangle * 3u]);
	mn_countTriangle = 0u;

	mf_mouvement = 0.00f;

	createSun();
}

void Sun::init(Biome * p_biome)
{
	Decor::init(p_biome);
	randomDecor();
}

void Sun::update(float pf_deltatime)
{
	Decor::update(pf_deltatime);

	createSun();
}
