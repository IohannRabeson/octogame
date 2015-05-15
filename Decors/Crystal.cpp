#include "Crystal.hpp"
#include "Map.hpp"

Crystal::Crystal() :
	Decor(),
	mn_countCrystal(0u),
	m_picCrystal(0u),
	m_nbCrystal(0u),
	mf_starTimer(0.f)
{
}

Crystal::~Crystal()
{
	m_refAngle.clear();
	m_refSize.clear();
	m_refColor.clear();
	m_up.clear();
}

sf::Vector2f Crystal::createOneCrystal(sf::Vector2f p_size, float pf_angle, sf::Color p_color)
{
	sf::Vector2f up(0.0f, -p_size.x - p_size.y);
	sf::Vector2f upLeft(-p_size.x, -p_size.y);
	sf::Vector2f upMid(0.0f, -p_size.y);
	sf::Vector2f upRight(p_size.x, -p_size.y);

	sf::Vector2f down(0.0f, p_size.x);
	sf::Vector2f downLeft(-p_size.x, 0.f);
	sf::Vector2f downMid(0.0f, 0.f);
	sf::Vector2f downRight(p_size.x, 0.f);

	float angle = pf_angle * PI / 180.0f;
	float cosA = cos(angle);
	float sinA = sin(angle);

	rotateVec(&up, cosA, sinA);
	rotateVec(&upLeft, cosA, sinA);
	rotateVec(&upMid, cosA, sinA);
	rotateVec(&upRight, cosA, sinA);
	rotateVec(&down, cosA, sinA);
	rotateVec(&downLeft, cosA, sinA);
	rotateVec(&downMid, cosA, sinA);
	rotateVec(&downRight, cosA, sinA);

	// Down right
	p_color += sf::Color(5, 5, 5);
	p_color.a -= 60;
	createTriangle(down, downMid, downRight, m_origin, p_color);

	// Mid right
	p_color += sf::Color(5, 5, 5);
	p_color.a -= 60;
	createRectangle(upRight, upMid, downMid, downRight, m_origin, p_color);

	// Up right
	p_color += sf::Color(5, 5, 5);
	p_color.a -= 60;
	createTriangle(up, upMid, upRight, m_origin, p_color);

	// Down left
	p_color += sf::Color(5, 5, 5);
	p_color.a -= 60;
	createTriangle(down, downMid, downLeft, m_origin, p_color);

	// Mid left
	p_color += sf::Color(5, 5, 5);
	p_color.a -= 60;
	createRectangle(upLeft, upMid, downMid, downLeft, m_origin, p_color);

	// Up left
	p_color += sf::Color(5, 5, 5);
	p_color.a -= 60;
	createTriangle(up, upMid, upLeft, m_origin, p_color);

	if (mf_starTimer >= 0.3f)
	{
		m_picCrystal = randomRange(0, 3);
		mf_starTimer = 0.f;
	}
	switch (m_picCrystal)
	{
		case 0:
			return up;
		case 1:
			return upLeft;
		case 2:
			return upRight;
		case 3:
			return upMid;
		default:
			return up;
	}
}

void Crystal::createCrystal(void)
{
	mn_countTriangle = 0u;
	for (int i = 0; i < mn_countCrystal; i++)
		m_up[i] = createOneCrystal(sf::Vector2f(m_refSize[i].x * mf_mouvement, m_refSize[i].y * mf_mouvement), m_refAngle[i], m_refColor[i]);
}

void Crystal::randomDecor(void)
{
	Decor::randomDecor();

	mn_countCrystal = randomRange(m_biome->m_crystal.mn_minElement, m_biome->m_crystal.mn_maxElement);
	mn_maxTriangle = 8 * mn_countCrystal;

	m_triangle.reset(new sf::Vertex[mn_maxTriangle * 3u]);
	m_refAngle.resize(mn_countCrystal);
	m_refSize.resize(mn_countCrystal);
	m_refColor.resize(mn_countCrystal);
	m_up.resize(mn_countCrystal);

	m_color = sf::Color(5.f, 103.f, 155.f);
	m_size = sf::Vector2f(randomRange(m_biome->m_crystal.mn_minSizeX, m_biome->m_crystal.mn_maxSizeX), randomRange(m_biome->m_crystal.mn_minSizeY, m_biome->m_crystal.mn_maxSizeY));

	for (int i = 0; i < mn_countCrystal; i++)
	{
		m_refSize[i].x = randomRange(m_size.x / 2.0f, m_size.x);
		m_refSize[i].y = randomRange(m_size.y / 2.0f, m_size.y);
		m_refAngle[i] = randomRange(-45 + (i * 90 / mn_countCrystal), -45 + ((i + 1) * 90 / mn_countCrystal));
		int deltaColor = randomRange(0, 80);
		m_refColor[i] = m_color + sf::Color(deltaColor, deltaColor, deltaColor);
	}
}

void Crystal::init(Biome * p_biome)
{
	Decor::init(p_biome);
	randomDecor();
	createCrystal();
	m_star.init(m_biome);
}

void Crystal::update(float pf_deltatime)
{
	Decor::update(pf_deltatime);
	Decor::putOnMap();
	m_origin.y += m_size.x;

	mf_starTimer += pf_deltatime;
	createCrystal();

	if (mf_starTimer == 0.f)
		m_nbCrystal = randomRange(0, mn_countCrystal);
	m_star.setOrigin(m_up[m_nbCrystal] + m_origin);
	m_star.update(pf_deltatime);
}

void Crystal::draw(sf::RenderTarget& p_target, sf::RenderStates p_states) const
{
	Decor::draw(p_target, p_states);
	m_star.draw(p_target, p_states);
}
