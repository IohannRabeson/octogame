#include "Cloud.hpp"
#include "Map.hpp"

Cloud::Cloud() :
	Decor(),
	mn_coundOctogon(0),
	mn_alpha(0)
{
}

Cloud::~Cloud()
{
	m_values.clear();
}

void Cloud::createOctogon(sf::Vector2f p_size, sf::Vector2f p_origin, sf::Color & p_color, float p_sizeUp, float p_sizeDown, float p_sizeRec)
{
	p_color.a = mf_mouvement * mn_alpha;

	sf::Vector2f upLeft(-p_size.x + p_sizeUp, -p_size.y - p_sizeUp);
	sf::Vector2f upRight(p_size.x - p_sizeUp, -p_size.y - p_sizeUp);
	sf::Vector2f cornerUpLeft(-p_size.x + p_sizeUp, -p_size.y);
	sf::Vector2f cornerUpRight(p_size.x - p_sizeUp, -p_size.y);
	sf::Vector2f upMidLeft(-p_size.x, -p_size.y);
	sf::Vector2f upMidRight(p_size.x, -p_size.y);

	sf::Vector2f downLeft(-p_size.x + p_sizeDown, p_size.y + p_sizeDown);
	sf::Vector2f downRight(p_size.x - p_sizeDown, p_size.y + p_sizeDown);
	sf::Vector2f cornerDownLeft(-p_size.x + p_sizeDown, p_size.y);
	sf::Vector2f cornerDownRight(p_size.x - p_sizeDown, p_size.y);
	sf::Vector2f downMidLeft(-p_size.x, p_size.y);
	sf::Vector2f downMidRight(p_size.x, p_size.y);

	sf::Vector2f recUpLeft(downMidLeft.x, downMidLeft.y - p_sizeRec);
	sf::Vector2f recUpRight(downMidLeft.x + 4.f, recUpLeft.y);
	sf::Vector2f recDown(downMidLeft.x + 4.f, downMidLeft.y);
	sf::Vector2f secondRec(downLeft.x, downLeft.y - 4.f);

	// Corner up
	createTriangle(upMidRight, cornerUpRight, upRight, p_origin, p_color);
	createTriangle(upMidLeft, cornerUpLeft, upLeft, p_origin, p_color);

	// Fill with rectangle
	createRectangle(upLeft, upRight, cornerUpRight, cornerUpLeft, p_origin, p_color);
	createRectangle(cornerUpLeft, cornerUpRight, cornerDownRight, cornerDownLeft, p_origin, p_color);
	createRectangle(cornerDownLeft, cornerDownRight, downRight, downLeft, p_origin, p_color);
	createRectangle(upMidLeft, cornerUpLeft, cornerDownLeft, downMidLeft, p_origin, p_color);
	createRectangle(cornerUpRight, upMidRight, downMidRight, cornerDownRight, p_origin, p_color);

	// Corner down
	createTriangle(downMidRight, cornerDownRight, downRight, p_origin, p_color);
	createTriangle(downMidLeft, cornerDownLeft, downLeft, p_origin, p_color);

	// Details
	createRectangle(downMidLeft, recUpLeft, recUpRight, recDown, p_origin, p_color);
	createRectangle(downMidLeft, recDown, secondRec, downLeft, p_origin, p_color);
}

void Cloud::createCloud(void)
{
	mn_countVertex = 0;
	for (int i = 0; i < mn_coundOctogon; i++)
		createOctogon(sf::Vector2f(m_values[i].size.x, m_values[i].size.y * mf_mouvement), m_values[i].origin + m_origin, m_color,
						m_values[i].sizeUp * mf_mouvement, m_values[i].sizeDown * mf_mouvement, m_values[i].sizeRec * mf_mouvement);
	if (b_isIce == true)
	{
		sf::Color iceColor(5, 103, 155, 60);
		for (int i = 0; i < mn_coundOctogon; i++)
			createOctogon(sf::Vector2f(m_values[i].size.x, m_values[i].size.y * mf_mouvement * 1.1f), m_values[i].origin + m_origin, iceColor,
							m_values[i].sizeUp * 1.2f, m_values[i].sizeDown * 1.2f, m_values[i].sizeRec * mf_mouvement * 1.2f);
	}
}

void Cloud::randomDecor(void)
{
	Decor::randomDecor();
	// Init values
	mf_liveTime = randomRange(m_biome->m_tree.mn_minLive, m_biome->m_tree.mn_maxLive);
	m_color = sf::Color(230, 230, 230);
	mn_alpha = randomRange(100, 220);
	m_size = sf::Vector2f(randomRange(m_biome->m_cloud.mn_minSizeX, m_biome->m_cloud.mn_maxSizeX), randomRange(m_biome->m_cloud.mn_minSizeY, m_biome->m_cloud.mn_maxSizeY));
	m_origin.x = randomRange(0, 1900);
	m_origin.y = randomRange(0, 500);

	// Allocate memory
	mn_coundOctogon = randomRange(m_biome->m_cloud.mn_minElement, m_biome->m_cloud.mn_maxElement);
	mn_maxTriangle = (18 * mn_coundOctogon) * 2;
	allocateVertex(mn_maxTriangle * 3u);
	mn_countVertex = 0u;

	m_values.resize(mn_coundOctogon);

	// Compute left random values
	int i = 0;
	float totalY = 0;
	sf::Vector2f size = m_size;
	sf::Vector2f origin = sf::Vector2f(0.f, 0.f);
	while (i < mn_coundOctogon / 2)
	{
		size.y = randomRange(m_biome->m_cloud.mn_minSizeY, m_biome->m_cloud.mn_maxSizeY);
		totalY += size.y;
		size.x -= totalY;
		origin.y += randomRange(static_cast<int>(-totalY), 0.f);
		if (size.y * 2 < size.x)
		{
			m_values[i].size = size;
			m_values[i].origin = origin;
			m_values[i].sizeUp = randomRange(static_cast<int>(size.y), static_cast<int>(size.y * 2));
			m_values[i].sizeDown = randomRange(static_cast<int>(size.y), static_cast<int>(size.y * 2));
			m_values[i].sizeRec = randomRange(10, static_cast<int>(size.y * 2));
		}
		else
			break;
		i++;
	}

	// Compute right random values
	totalY = 0;
	size = m_size;
	origin = sf::Vector2f(0.f, 0.f + m_size.y);
	while (i < mn_coundOctogon)
	{
		size.y = randomRange(m_biome->m_cloud.mn_minSizeY, m_biome->m_cloud.mn_maxSizeY);
		totalY += size.y;
		size.x -= totalY;
		origin.y += randomRange(0.0f, static_cast<int>(totalY));
		if (size.y * 2 < size.x)
		{
			m_values[i].size = size;
			m_values[i].origin = origin;
			m_values[i].sizeUp = randomRange(static_cast<int>(size.y), static_cast<int>(size.y * 2));
			m_values[i].sizeDown = randomRange(static_cast<int>(size.y), static_cast<int>(size.y * 2));
			m_values[i].sizeRec = randomRange(10, static_cast<int>(size.y * 2));
		}
		i++;
	}

	createCloud();
}

void Cloud::init(Biome * p_biome)
{
	Decor::init(p_biome);
	randomDecor();
	m_values.reserve(m_biome->m_cloud.mn_maxElement);
}

void Cloud::update(float pf_deltatime)
{
	Decor::update(pf_deltatime);

	if (b_isIce == false)
		m_origin.x += pf_deltatime * m_biome->mn_wind;

	if (m_origin.x >= 1900 + 500)
	{
		randomDecor();
		m_origin.x = -500;
	}

	createCloud();
}
