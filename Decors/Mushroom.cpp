#include "Mushroom.hpp"
#include "Map.hpp"

Mushroom::Mushroom() :
	Decor()
{
}

Mushroom::~Mushroom()
{
}

void Mushroom::createOneMushroom(sf::Vector2f p_size, sf::Vector2f p_origin, sf::Color p_color)
{
	sf::Vector2f upLeft(-p_size.x / 2.f, -p_size.y);
	sf::Vector2f upRight(p_size.x / 2.f, -p_size.y);
	sf::Vector2f downLeft(-p_size.x / 2.f, 0.f);
	sf::Vector2f downRight(p_size.x / 2.f, 0.f);
	createRectangle(upLeft, upRight, downRight, downLeft, p_origin, p_color);
}

void Mushroom::createMushroom(void)
{
	mn_countTriangle = 0;

	createOneMushroom(m_size * mf_mouvement, m_origin, m_color);
}

void Mushroom::randomDecor(void)
{
	Decor::randomDecor();
	m_color = sf::Color(255, 255, 255);
	float size = randomRange(m_biome->m_sun.mn_minSizeX, m_biome->m_sun.mn_maxSizeX);
	m_size = sf::Vector2f(size, size);
	m_origin = sf::Vector2f(400, 400);

	// Init containers
	mn_maxTriangle = 10 * (m_biome->m_sun.mn_nb + 2);
	allocateVertex(mn_maxTriangle * 3u);
	createMushroom();
}

void Mushroom::init(Biome * p_biome)
{
	Decor::init(p_biome);
	randomDecor();
	createMushroom();
}

void Mushroom::update(float pf_deltatime)
{
	Decor::update(pf_deltatime);
	createMushroom();
}
