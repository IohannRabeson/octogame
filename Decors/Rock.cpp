#include "Rock.hpp"
#include "Map.hpp"

Rock::Rock() :
	Decor()
{
}

Rock::~Rock()
{
	m_refSize.clear();
	m_refOrigin.clear();
	m_refSizeLeft.clear();
	m_refSizeRight.clear();
	m_refSizeRec.clear();
}

void Rock::createOneRock(sf::Vector2f p_size, sf::Vector2f p_origin, sf::Color p_color, float p_sizeLeft, float p_sizeRight, float p_sizeRec)
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
	createRectangle(upLeft, upRight, downMidRight, downMidLeft, p_origin, p_color);
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
	mn_countTriangle = 0;
	for (int i = 0; i < mn_countRock; i++)
		createOneRock(sf::Vector2f(m_refSize[i].x, m_refSize[i].y * mf_mouvement), m_refOrigin[i] + m_origin, m_color,
						m_refSizeLeft[i], m_refSizeRight[i], m_refSizeRec[i] * mf_mouvement);
	createTriangle(m_left, m_right, sf::Vector2f(0.0f, (m_right.x - m_left.x) / 2.f), m_origin, m_color);
	/*
	sf::Color iceColor(5, 103, 155, 60);
	if (me_currentState == e_state_sleep)
	{
		for (int i = 0; i < mn_countRock; i++)
			createOneRock(sf::Vector2f(m_refSize[i].x, m_refSize[i].y * mf_mouvement * 1.1f), m_refOrigin[i] + m_origin, iceColor,
							m_refSizeLeft[i] * 1.2f, m_refSizeRight[i] * 1.2f, m_refSizeRec[i] * mf_mouvement * 1.2f);
		createTriangle(m_left, m_right, sf::Vector2f(0.0f, (m_right.x - m_left.x) / 2.f), m_origin, iceColor);
	}
	*/
}

void Rock::randomDecor(void)
{
	Decor::randomDecor();
	m_color = sf::Color(122.f, 108.f, 135.f);
	m_size = sf::Vector2f(randomRange(m_biome->m_rock.mn_minSizeX, m_biome->m_rock.mn_maxSizeX), randomRange(m_biome->m_rock.mn_minSizeY, m_biome->m_rock.mn_maxSizeY));

	// Init containers
	mn_countRock = randomRange(m_biome->m_rock.mn_minElement, m_biome->m_rock.mn_maxElement);
	mn_maxTriangle = (10 * mn_countRock + 1) * 2; // +1 for root triangle
	allocateVertex(mn_maxTriangle * 3u);
	mn_countTriangle = 0u;

	m_refSize.resize(mn_countRock);
	m_refOrigin.resize(mn_countRock);
	m_refSizeLeft.resize(mn_countRock);
	m_refSizeRight.resize(mn_countRock);
	m_refSizeRec.resize(mn_countRock);

	m_left = sf::Vector2f(0.f, 0.f);
	m_right = sf::Vector2f(0.f, 0.f);

	int i = 0;
	// Compute left random values
	float totalX = 0;
	sf::Vector2f size = m_size;
	sf::Vector2f origin = sf::Vector2f(0.f, 0.f);
	while (i < mn_countRock / 2)
	{
		size.x = randomRange(m_biome->m_rock.mn_minSizeX, m_biome->m_rock.mn_maxSizeX);
		totalX += size.x;
		size.y -= totalX;
		origin.x += randomRange(static_cast<int>(-totalX), 0.f);
		if (size.x * 2 < size.y)
		{
			m_refSize[i] = size;
			m_refOrigin[i] = origin;
			m_refSizeLeft[i] = randomRange(static_cast<int>(size.x), static_cast<int>(size.x * 2));
			m_refSizeRight[i] = randomRange(static_cast<int>(size.x), static_cast<int>(size.x * 2));
			m_refSizeRec[i] = randomRange(10, static_cast<int>(size.x * 2));
		}
		else
			break;
		i++;
	}

	// Compute right random values
	totalX = 0;
	size = m_size;
	origin = sf::Vector2f(0.f + m_size.x, 0.f);
	while (i < mn_countRock)
	{
		size.x = randomRange(m_biome->m_rock.mn_minSizeX, m_biome->m_rock.mn_maxSizeX);
		totalX += size.x;
		size.y -= totalX;
		origin.x += randomRange(0.0f, static_cast<int>(totalX));
		if (size.x * 2 < size.y)
		{
			m_refSize[i] = size;
			m_refOrigin[i] = origin;
			m_refSizeLeft[i] = randomRange(static_cast<int>(size.x), static_cast<int>(size.x * 2));
			m_refSizeRight[i] = randomRange(static_cast<int>(size.x), static_cast<int>(size.x * 2));
			m_refSizeRec[i] = randomRange(10, static_cast<int>(size.x * 2));
		}
		i++;
	}
	mf_mouvement = 0.00f;

	createRock();
}

void Rock::init(Biome * p_biome)
{
	Decor::init(p_biome);
	randomDecor();
}

void Rock::update(float pf_deltatime)
{
	Decor::update(pf_deltatime);
	Decor::putOnMap();
	m_origin.y += m_size.x * 4;

	createRock();
}
