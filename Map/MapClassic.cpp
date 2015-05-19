#include "MapClassic.hpp"
#include "OctoNoise.hpp"

MapClassic::MapClassic(unsigned int pn_width, unsigned int pn_height) :
	Map(pn_width, pn_height)
{}

MapClassic::~MapClassic(void) { }

void MapClassic::initBiome(void)
{
	m_biome->mn_height = 68;
	m_biome->mn_width = 358;
	m_biome->mf_transitionTimerMax = 0.4f;
	m_biome->mn_nbDecor = 10;
	mn_totalWidth = m_biome->mn_width;
}

float MapClassic::firstCurve(float * vec)
{
	vec[0] /= 100.0;
	vec[1] /= 100.f;
	return OctoNoise::getCurrent().fbm(vec);
}

float MapClassic::secondCurve(float * vec)
{
	vec[0] /= 70.0;
	vec[1] /= 10.0;
	vec[2] /= 10.f;
	return OctoNoise::getCurrent().noise3(vec);
}

void MapClassic::setColor(Tile & p_tile)
{
	p_tile.m_startColor = sf::Color(128.f, 0.f, 128.f);
}

void MapClassic::nextStep(void)
{
	mf_depth += 2.f;
}

void MapClassic::previousStep(void)
{
	mf_depth -= 2.f;
}
