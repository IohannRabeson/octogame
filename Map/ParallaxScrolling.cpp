#include "ParallaxScrolling.hpp"
#include "ABiome.hpp"

ParallaxScrolling::ParallaxScrolling(std::initializer_list<ALayer *> list)
{
	for (auto & layer : list)
		addLayer(layer);
}

ParallaxScrolling::~ParallaxScrolling(void)
{
	removeAllLayers();
}

void ParallaxScrolling::setup(ABiome & biome)
{
	// Clean existing layers
	removeAllLayers();
	// Add new layers
	addLayer(biome.getLayers());
}

void ParallaxScrolling::addLayer(ALayer * layer)
{
	if (!layer)
		return;
	layer->init();
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
	for (auto & layer : m_layers)
		layer->update(deltatime);
}

void ParallaxScrolling::draw(sf::RenderTarget & render, sf::RenderStates states) const
{
	for (auto const & layer : m_layers)
		layer->draw(render, states);
}
