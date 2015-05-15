#include "MapInstance.hpp"
#include "OctoNoise.hpp"
#include <cmath>

MapInstance::MapInstance(unsigned int pn_width, unsigned int pn_height) :
	Map(pn_width, pn_height),
	m_instance(nullptr)
{}

MapInstance::~MapInstance(void) { }

void MapInstance::initBiome(void)
{
	m_biome->mn_height = 128u;
	m_biome->mn_width = 512u;
	m_biome->mf_transitionTimerMax = 0.5f;
	m_biome->mn_nbDecor = 15u;
	m_biome->mn_temperature = -1;
	m_instance = new StaticTileObject(140u, 40u, 5u);
	m_instance->load();
	mn_totalWidth = m_biome->mn_width + m_instance->getWidth();
}

#include <iostream>
void MapInstance::computeMapRange(int p_startX, int p_endX, int p_startY, int p_endY)
{
	float vec[3];
	int height, offsetInstance;
	unsigned int offset;
	float v;
	bool inInstance;

	for (int x = p_startX; x < p_endX; x++)
	{
		inInstance = false;
		offset = x + mn_offsetX;
		if (offset >= m_biome->mn_width)
		{
			if (offset < mn_totalWidth)
				inInstance = true;
			else if (offset >= mn_totalWidth)
				offset -= mn_totalWidth;
		}

		if (inInstance)
			vec[0] = static_cast<float>(m_biome->mn_width);
		else
			vec[0] = static_cast<float>(offset);

		vec[1] = mf_depth;
		// firstCurve return a value between -1 & 1
		// we normalize it betwen 0 & max_height
		v = (firstCurve(vec) + 1.f) * static_cast<float>(m_biome->mn_height) / 2.f;
		offsetInstance = height = static_cast<int>(v) - mn_offsetY;
		if (height < 0)
			height = 0;
		else if (height > p_endY)
			height = p_endY;

		for (int y = height; y < p_endY; y++)
		{
			float yy = y - offsetInstance;
			if (inInstance && yy >= 0 && yy < m_instance->getHeight())
				m_tiles.get(x, y)->mb_isEmpty = m_instance->get(offset - m_biome->mn_width, yy).mb_isEmpty;
			else
				m_tiles.get(x, y)->mb_isEmpty = false;
			vec[0] = static_cast<float>(offset);
			vec[1] = static_cast<float>(y + mn_offsetY);
			vec[2] = mf_depth;
			m_tiles.get(x, y)->mf_noiseValue = (secondCurve(vec) + 1.f) / 2.f;
			setColor(*m_tiles.get(x, y));
		}
		for (int y = p_startY; y < height; y++)
			m_tiles.get(x, y)->mb_isEmpty = true;
	}
}

float MapInstance::firstCurve(float * vec)
{
	vec[0] /= 100.f;
	vec[1] /= 100.f;
	return OctoNoise::getCurrent().fbm(vec, 3, 2.0f, 0.4f);
}

float MapInstance::secondCurve(float * vec)
{
	vec[0] /= 10.f;
	vec[1] /= 10.f;
	vec[2] /= 10.f;
	return OctoNoise::getCurrent().noise3(vec);
	//return sin(vec[0] * 15.f + OctoNoise::getCurrent().noise3(vec) * sin(vec[1]) * 5.f);
}

// TODO: To be deleted
void lerpC(sf::Color & p_result, sf::Color & p_start, sf::Color & p_end, float p_transition)
{
	p_result.r = p_start.r * (1.f - p_transition) + p_end.r * p_transition;
	p_result.g = p_start.g * (1.f - p_transition) + p_end.g * p_transition;
	p_result.b = p_start.b * (1.f - p_transition) + p_end.b * p_transition;
}

void MapInstance::setColor(Tile & p_tile)
{
	sf::Color start = sf::Color(178.f, 0.f, 86.f);
	sf::Color end = sf::Color(178.f, 162.f, 32.f);
	sf::Color mid = sf::Color(0.f, 74.f, 213.f);

	float changeColor = static_cast<float>(mn_offsetX) / mn_totalWidth;
	lerpC(start, start, mid, changeColor);
	lerpC(end, end, mid, p_tile.mf_noiseValue);

	if (p_tile.mf_noiseValue < 0.4f)
		p_tile.m_startColor = start;
	else if (p_tile.mf_noiseValue < 0.6f)
	{
		lerpC(p_tile.m_startColor, start, end, (p_tile.mf_noiseValue - 0.4f) * 5.f);
	}
	else
		p_tile.m_startColor = end;
}

void MapInstance::swapDepth(void)
{
	float tmp = mf_depth;
	mf_depth = mf_oldDepth;
	mf_oldDepth = tmp;
	m_instance->swapDepth();
}

void MapInstance::registerDepth(void)
{
	mf_oldDepth = mf_depth;
	m_instance->registerDepth();
}

void MapInstance::nextStep(void)
{
	mf_depth += 3.f;
	m_instance->nextStep();
}

void MapInstance::previousStep(void)
{
	mf_depth -= 3.f;
	m_instance->previousStep();
}
