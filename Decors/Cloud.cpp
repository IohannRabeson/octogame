#include "Cloud.hpp"
#include "Map.hpp"

Cloud::Cloud() :
	Decor()
{
}

Cloud::~Cloud()
{
	m_refSize.clear();
	m_refOrigin.clear();
	m_refSizeLeft.clear();
	m_refSizeRight.clear();
	m_refSizeRec.clear();
}

void Cloud::createOneCloud(sf::Vector2f p_size, sf::Vector2f p_origin, sf::Color p_color, float p_sizeLeft, float p_sizeRight, float p_sizeRec)
{
	sf::Vector2f upLeft(-p_size.x, -p_size.y);
	sf::Vector2f upRight(p_size.x, -p_size.y);
	sf::Vector2f cornerUpLeft(-p_size.x, -p_size.y + p_sizeLeft);
	sf::Vector2f cornerUpRight(p_size.x, -p_size.y + p_sizeRight);
	sf::Vector2f upMidLeft(-p_size.x - p_sizeLeft, -p_size.y + p_sizeLeft);
	sf::Vector2f upMidRight(p_size.x + p_sizeRight, -p_size.y + p_sizeRight);

	sf::Vector2f downLeft(-p_size.x, p_size.y);
	sf::Vector2f downRight(p_size.x, p_size.y);
	sf::Vector2f cornerDownLeft(-p_size.x, p_size.y - p_sizeLeft);
	sf::Vector2f cornerDownRight(p_size.x, p_size.y - p_sizeRight);
	sf::Vector2f downMidLeft(-p_size.x - p_sizeLeft, p_size.y - p_sizeLeft);
	sf::Vector2f downMidRight(p_size.x + p_sizeRight, p_size.y - p_sizeRight);

	sf::Vector2f recUp(upLeft.x + p_sizeRec, upLeft.y);
	sf::Vector2f recLeft(upLeft.x, upLeft.y + 4.0f);
	sf::Vector2f recRight(recUp.x, recUp.y + 4.0f);

	float cosA = cos(-90.f * PI / 180.f);
	float sinA = sin(-90.f * PI / 180.f);
	rotateVec(&upLeft, cosA, sinA);
	rotateVec(&upRight, cosA, sinA);
	rotateVec(&cornerUpLeft, cosA, sinA);
	rotateVec(&cornerUpRight, cosA, sinA);
	rotateVec(&upMidLeft, cosA, sinA);
	rotateVec(&upMidRight, cosA, sinA);
	rotateVec(&downLeft, cosA, sinA);
	rotateVec(&downRight, cosA, sinA);
	rotateVec(&cornerDownLeft, cosA, sinA);
	rotateVec(&cornerDownRight, cosA, sinA);
	rotateVec(&downMidLeft, cosA, sinA);
	rotateVec(&downMidRight, cosA, sinA);
	rotateVec(&recUp, cosA, sinA);
	rotateVec(&recLeft, cosA, sinA);
	rotateVec(&recRight, cosA, sinA);

	createTriangle(upMidRight, cornerUpRight, upRight, p_origin, p_color);
	sf::Color tmpColor = p_color;
	createTriangle(upMidLeft, cornerUpLeft, upLeft, p_origin, tmpColor);

	createRectangle(upLeft, upRight, downRight, downLeft, p_origin, p_color);
	createRectangle(upMidLeft, cornerUpLeft, cornerDownLeft, downMidLeft, p_origin, p_color);
	createRectangle(upMidRight, cornerUpRight, cornerDownRight, downMidRight, p_origin, p_color);

	createTriangle(downMidRight, cornerDownRight, downRight, p_origin, p_color);
	createTriangle(downMidLeft, cornerDownLeft, downLeft, p_origin, p_color);

	createRectangle(upLeft, recUp, recRight, recLeft, p_origin, tmpColor);
}

void Cloud::createCloud(void)
{
	mn_countTriangle = 0;
	for (int i = 0; i < mn_countCloud; i++)
		createOneCloud(sf::Vector2f(m_refSize[i].x * mf_mouvement, m_refSize[i].y * mf_mouvement), m_refOrigin[i] + m_origin, m_color,
						m_refSizeLeft[i] * mf_mouvement, m_refSizeRight[i] * mf_mouvement, m_refSizeRec[i] * mf_mouvement);
	/*
	sf::Color iceColor(5, 103, 155, 60);
	if (me_currentState == e_state_sleep)
	{
		for (int i = 0; i < mn_countCloud; i++)
			createOneCloud(sf::Vector2f(m_refSize[i].x, m_refSize[i].y * mf_mouvement * 1.1f), m_refOrigin[i] + m_origin, iceColor,
							m_refSizeLeft[i] * 1.2f, m_refSizeRight[i] * 1.2f, m_refSizeRec[i] * mf_mouvement * 1.2f);
	}
	*/
}

void Cloud::randomDecor(void)
{
	Decor::randomDecor();
	m_color = sf::Color(230.f, 230.f, 230.f);
	m_color.a = randomRange(100, 220);
	m_size = sf::Vector2f(randomRange(m_biome->m_cloud.mn_minSizeX, m_biome->m_cloud.mn_maxSizeX), randomRange(m_biome->m_cloud.mn_minSizeY, m_biome->m_cloud.mn_maxSizeY));
	m_origin.x = randomRange(0, 1900);
	m_origin.y = randomRange(0, 500);

	// Init containers
	mn_countCloud = randomRange(m_biome->m_cloud.mn_minElement, m_biome->m_cloud.mn_maxElement);
	mn_maxTriangle = (14 * mn_countCloud); // +1 for root triangle
	allocateVertex(mn_maxTriangle * 3u);
	mn_countTriangle = 0u;

	m_refSize.resize(mn_countCloud);
	m_refOrigin.resize(mn_countCloud);
	m_refSizeLeft.resize(mn_countCloud);
	m_refSizeRight.resize(mn_countCloud);
	m_refSizeRec.resize(mn_countCloud);

	int i = 0;
	// Compute left random values
	float totalX = 0;
	sf::Vector2f size = m_size;
	sf::Vector2f origin = sf::Vector2f(0.f, 0.f);
	while (i < mn_countCloud / 2)
	{
		size.x = randomRange(m_biome->m_cloud.mn_minSizeX, m_biome->m_cloud.mn_maxSizeX);
		totalX += size.x;
		size.y -= totalX;
		origin.y += randomRange(static_cast<int>(-totalX), 0.f);
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
	while (i < mn_countCloud)
	{
		size.x = randomRange(m_biome->m_cloud.mn_minSizeX, m_biome->m_cloud.mn_maxSizeX);
		totalX += size.x;
		size.y -= totalX;
		origin.y += randomRange(0.0f, static_cast<int>(totalX));
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

	createCloud();
}

void Cloud::init(Biome * p_biome)
{
	Decor::init(p_biome);
	randomDecor();
}

void Cloud::update(float pf_deltatime)
{
	Decor::update(pf_deltatime);

	m_origin.x += pf_deltatime * 100.f * mf_mouvement;
	if (m_origin.x >= 1900 + 500)
	{
		randomDecor();
		m_origin.x = -500;
	}

	createCloud();
}
