#include "ParallaxScrolling.hpp"
#include "ABiome.hpp"
#include "SkyCycle.hpp"
#include "Progress.hpp"

ParallaxScrolling::ParallaxScrolling(void) :
	m_biome(nullptr),
	m_skyCycle(nullptr)
{}

ParallaxScrolling::~ParallaxScrolling(void)
{
	removeAllLayers();
}

void ParallaxScrolling::setup(ABiome & biome, SkyCycle & skyCycle)
{
	m_biome = &biome;
	m_skyCycle = &skyCycle;

	// Clean existing layers
	removeAllLayers();
	// Add new layers
	addLayer(biome.getLayers());
}

void ParallaxScrolling::addLayer(ALayer * layer)
{
	if (!layer)
		return;
	layer->setup();
	layer->setOwner(this);
	m_layers.push_back(std::unique_ptr<ALayer>(layer));
}

void ParallaxScrolling::addLayer(std::vector<ALayer *> const & layers)
{
	for (auto & layer : layers)
		addLayer(layer);
}

void ParallaxScrolling::removeLayer(std::size_t index)
{
	if (index >= m_layers.size())
		return;
	m_layers.erase(m_layers.begin() + index);
}

void ParallaxScrolling::removeAllLayers(void)
{
	m_layers.clear();
}

void ParallaxScrolling::update(float deltatime)
{
	if (Progress::getInstance().getLevelOfDetails() >= -1)
	{
		for (auto & layer : m_layers)
			layer->update(deltatime, *m_biome);
	}
}

void ParallaxScrolling::draw(sf::RenderTarget & render, sf::RenderStates states) const
{
	if (Progress::getInstance().getLevelOfDetails() >= -1)
	{
		for (auto const & layer : m_layers)
			layer->draw(render, states);
	}
}
