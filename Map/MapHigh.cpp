#include "MapHigh.hpp"
#include "OctoNoise.hpp"
#include <cmath>

MapHigh::MapHigh(void) :
	Map()
{}

MapHigh::~MapHigh(void) { }

void MapHigh::initBiome(void)
{
	m_biome->mn_height = 128;
	m_biome->mn_width = 512;
	m_biome->mf_transitionTimerMax = 1.1f;
	m_biome->mn_temperature = 30;
	m_biome->mn_nbDecor = 10;
	mn_totalWidth = m_biome->mn_width;
}

float MapHigh::firstCurve(float * vec)
{
	vec[0] /= 100.f;
	vec[1] /= 100.f;
	return OctoNoise::getCurrent().fbm(vec, 3, 2.0f, 0.4f);
}

float MapHigh::secondCurve(float * vec)
{
	vec[0] /= 70.f;
	vec[1] /= 10.f;
	vec[2] /= 10.f;
	return sin(vec[0] * 15.f + OctoNoise::getCurrent().noise3(vec) * sin(vec[1]) * 5.f);
}

// TODO: To be deleted
void lerp(sf::Color & p_result, sf::Color & p_start, sf::Color & p_end, float p_transition)
{
	p_result.r = p_start.r * (1.f - p_transition) + p_end.r * p_transition;
	p_result.g = p_start.g * (1.f - p_transition) + p_end.g * p_transition;
	p_result.b = p_start.b * (1.f - p_transition) + p_end.b * p_transition;
}

void MapHigh::setColor(Tile & p_tile)
{
	sf::Color start = sf::Color(180.f, 33.f, 85.f);
	sf::Color end = sf::Color(212.f, 185.f, 39.f);
	lerp(p_tile.m_startColor, start, end, p_tile.mf_noiseValue);
}

void MapHigh::nextStep(void)
{
	m_depth += 3.f;
}

void MapHigh::previousStep(void)
{
	m_depth -= 3.f;
}
