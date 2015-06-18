#include "Rock.hpp"
#include "Map.hpp"

Rock::Rock() :
	Decor(),
	mn_countOctogon(0)
{
}

Rock::~Rock()
{
	m_values.clear();
}

void Rock::createOctogon(sf::Vector2f p_size, sf::Vector2f p_origin, sf::Color p_color, float p_sizeLeft, float p_sizeRight, float p_sizeRec)
{
	sf::Vector2f upLeft(-p_size.x, -p_size.y);
	sf::Vector2f upRight(p_size.x, -p_size.y);
	sf::Vector2f left(-p_size.x - p_sizeLeft, -p_size.y + p_sizeLeft);
	sf::Vector2f right(p_size.x + p_sizeRight, -p_size.y + p_sizeRight);
	sf::Vector2f midLeft(-p_size.x, -p_size.y + p_sizeLeft);
	sf::Vector2f midRight(p_size.x, -p_size.y + p_sizeRight);
	sf::Vector2f downLeft(-p_size.x - p_sizeLeft, 0.f);
	sf::Vector2f downRight(p_size.x + p_sizeRight, 0.f);
	sf::Vector2f downMidLeft(-p_size.x, 0.f);
	sf::Vector2f downMidRight(p_size.x, 0.f);
	sf::Vector2f recUp(upLeft.x + p_sizeRec, upLeft.y);
	sf::Vector2f recLeft(upLeft.x, upLeft.y + 4.0f);
	sf::Vector2f recRight(recUp.x, recUp.y + 4.0f);

	// Avoid under limit point when grows
	midLeft.y = midLeft.y > 0.f ? 0.f : midLeft.y;
	left.y = left.y > 0.f ? 0.f : left.y;
	midRight.y = midRight.y > 0.f ? 0.f : midRight.y;
	right.y = right.y > 0.f ? 0.f : right.y;

	createTriangle(right, midRight, upRight, p_origin, p_color);


	createRectangle(upLeft, upRight, midRight, midLeft, p_origin, p_color);
	createRectangle(midLeft, midRight, downMidRight, downMidLeft, p_origin, p_color);
	createRectangle(left, midLeft, downMidLeft, downLeft, p_origin, p_color);
	createRectangle(right, midRight, downMidRight, downRight, p_origin, p_color);

	createTriangle(left, midLeft, upLeft, p_origin, p_color + sf::Color(100, 100, 100));
	createRectangle(upLeft, recUp, recRight, recLeft, p_origin, p_color + sf::Color(100, 100, 100));

	// Compute last left point
	if (p_origin.x - m_origin.x + downLeft.x < m_left.x && p_origin.x - m_origin.x < 0.f)
		m_left.x = p_origin.x - m_origin.x + downLeft.x;
	// Compute last right point
	if (p_origin.x - m_origin.x + downRight.x > m_right.x && p_origin.x - m_origin.x > 0.f)
		m_right.x = p_origin.x - m_origin.x + downRight.x;
}

void Rock::createRock(void)
{
	mn_countVertex = 0;
	for (int i = 0; i < mn_countOctogon; i++)
		createOctogon(sf::Vector2f(m_values[i].size.x, m_values[i].size.y * mf_mouvement), m_values[i].origin + m_origin, m_color,
						m_values[i].sizeLeft, m_values[i].sizeRight, m_values[i].sizeRec * mf_mouvement);
	createTriangle(m_left, m_right, sf::Vector2f(0.0f, (m_right.x - m_left.x) / 2.f), m_origin, m_color);

	if (b_isIce == true)
	{
		sf::Color iceColor(5, 103, 155, 60);
		if (me_currentState == e_state_sleep)
		{
			for (int i = 0; i < mn_countOctogon; i++)
			createOctogon(sf::Vector2f(m_values[i].size.x, m_values[i].size.y * mf_mouvement * 1.1f), m_values[i].origin + m_origin, iceColor,
							m_values[i].sizeLeft * 1.2f, m_values[i].sizeRight * 1.2f, m_values[i].sizeRec * mf_mouvement * 1.2f);
			createTriangle(m_left, m_right, sf::Vector2f(0.0f, (m_right.x - m_left.x) / 2.f), m_origin, iceColor);
		}
	}
}

void Rock::randomDecor(void)
{
	Decor::randomDecor();
	m_color = sf::Color(122.f, 108.f, 135.f);
	m_size = sf::Vector2f(randomRange(m_biome->m_rock.mn_minSizeX, m_biome->m_rock.mn_maxSizeX), randomRange(m_biome->m_rock.mn_minSizeY, m_biome->m_rock.mn_maxSizeY));

	// Allocate memory
	mn_countOctogon = randomRange(m_biome->m_rock.mn_minElement, m_biome->m_rock.mn_maxElement);
	mn_maxTriangle = (12 * mn_countOctogon + 1) * 2; // +1 for root triangle
	allocateVertex(mn_maxTriangle * 3u);
	mn_countVertex = 0u;

	m_values.resize(mn_countOctogon);

	m_left = sf::Vector2f(0.f, 0.f);
	m_right = sf::Vector2f(0.f, 0.f);

	// Compute left random values
	int i = 0;
	float totalX = 0;
	sf::Vector2f size = m_size;
	sf::Vector2f origin = sf::Vector2f(0.f, 0.f);
	while (i < mn_countOctogon / 2)
	{
		size.x = randomRange(m_biome->m_rock.mn_minSizeX, m_biome->m_rock.mn_maxSizeX);
		totalX += size.x;
		size.y -= totalX;
		origin.x += randomRange(static_cast<int>(-totalX), 0.f);
		if (size.x * 2 < size.y)
		{
			m_values[i].size = size;
			m_values[i].origin = origin;
			m_values[i].sizeLeft = randomRange(static_cast<int>(size.x), static_cast<int>(size.x * 2));
			m_values[i].sizeRight = randomRange(static_cast<int>(size.x), static_cast<int>(size.x * 2));
			m_values[i].sizeRec = randomRange(10, static_cast<int>(size.x * 2));
		}
		else
			break;
		i++;
	}

	// Compute right random values
	totalX = 0;
	size = m_size;
	origin = sf::Vector2f(0.f + m_size.x, 0.f);
	while (i < mn_countOctogon)
	{
		size.x = randomRange(m_biome->m_rock.mn_minSizeX, m_biome->m_rock.mn_maxSizeX);
		totalX += size.x;
		size.y -= totalX;
		origin.x += randomRange(0.0f, static_cast<int>(totalX));
		if (size.x * 2 < size.y)
		{
			m_values[i].size = size;
			m_values[i].origin = origin;
			m_values[i].sizeLeft = randomRange(static_cast<int>(size.x), static_cast<int>(size.x * 2));
			m_values[i].sizeRight = randomRange(static_cast<int>(size.x), static_cast<int>(size.x * 2));
			m_values[i].sizeRec = randomRange(10, static_cast<int>(size.x * 2));
		}
		i++;
	}

	createRock();
}

void Rock::init(Biome * p_biome)
{
	Decor::init(p_biome);
	randomDecor();
	m_values.reserve(m_biome->m_rock.mn_maxElement);
}

void Rock::update(float pf_deltatime)
{
	Decor::update(pf_deltatime);
	Decor::putOnMap();
	m_origin.y += m_size.x * 4;

	createRock();
}
