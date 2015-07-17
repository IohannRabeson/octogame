#include "ParallaxScrolling.hpp"

ParallaxScrolling::ParallaxScrolling(void)
{}

ParallaxScrolling::ParallaxScrolling(std::initializer_list<ALayer *> list) :
	m_layers(list)
{}

ParallaxScrolling::~ParallaxScrolling(void)
{
	removeAllLayers();
}

void ParallaxScrolling::addLayer(ALayer * layer)
{
	if (!layer)
		return;
	m_layers.push_back(layer);
}

void ParallaxScrolling::removeLayer(std::size_t index)
{
	if (index >= m_layers.size())
		return;
	ALayer * layer = m_layers[index];
	m_layers.erase(m_layers.begin() + index);
	delete layer;
}

void ParallaxScrolling::removeAllLayers(void)
{
	for (auto i = m_layers.begin(); i != m_layers.end(); i++)
		delete (*i);
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
