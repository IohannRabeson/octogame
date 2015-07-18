#include "ParallaxScrolling.hpp"

ParallaxScrolling::ParallaxScrolling(std::initializer_list<ALayer *> list)
{
	for (auto & layer : list)
		addLayer(layer);
}

ParallaxScrolling::~ParallaxScrolling(void)
{
	removeAllLayers();
}

void ParallaxScrolling::addLayer(ALayer * layer)
{
	if (!layer)
		return;
	layer->init();
	m_layers.push_back(std::move(std::unique_ptr<ALayer>(layer)));
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
