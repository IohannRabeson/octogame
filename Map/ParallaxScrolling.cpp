#include "ParallaxScrolling.hpp"
#include "ABiome.hpp"

ParallaxScrolling::~ParallaxScrolling(void)
{
	removeAllLayers();
}

void ParallaxScrolling::setup(ABiome & biome)
{
	// Clean existing layers
	removeAllLayers();
	// Add new layers
	addLayer(biome.getLayers(), biome);
}

void ParallaxScrolling::addLayer(ALayer * layer, ABiome & biome)
{
	if (!layer)
		return;
	layer->setup(biome);
	m_layers.push_back(std::move(std::unique_ptr<ALayer>(layer)));
}

void ParallaxScrolling::addLayer(std::vector<ALayer *> const & layers, ABiome & biome)
{
	for (auto & layer : layers)
		addLayer(layer, biome);
}

void ParallaxScrolling::removeLayer(std::size_t index)
{
	if (index >= m_layers.size())
		return;
	m_layers.erase(m_layers.begin() + index);
}

void ParallaxScrolling::setColor(sf::Color const & color)
{
	for (auto & layer : m_layers)
		layer->setOpacityColor(color);
}

void ParallaxScrolling::removeAllLayers(void)
{
	m_layers.clear();
}

void ParallaxScrolling::update(float deltatime)
{
	for (auto & layer : m_layers)
		layer->update(deltatime);
}

void ParallaxScrolling::draw(sf::RenderTarget & render, sf::RenderStates states) const
{
	for (auto const & layer : m_layers)
		layer->draw(render, states);
}
