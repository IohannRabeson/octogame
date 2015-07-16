#include "ParallaxScrolling.hpp"

ParallaxScrolling::ParallaxScrolling(std::initializer_list<ALayer *> list) :
	m_layers(list)
{}

ParallaxScrolling::~ParallaxScrolling(void)
{
	for (auto it = m_layers.begin(); it != m_layers.end(); it++)
		delete (*it);
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
