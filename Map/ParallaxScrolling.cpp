#include "ParallaxScrolling.hpp"
#include <Application.hpp>
#include <Camera.hpp>

ParallaxScrolling::ParallaxScrolling(void) :
	m_camera(octo::Application::getCamera())
{}

ParallaxScrolling::ParallaxScrolling(std::initializer_list<ALayer *> list) :
	m_camera(octo::Application::getCamera()),
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
	{
		if (layer->isScrolling())
			layer->updateOffset(m_camera.getRectangle().left);
		layer->update(deltatime);
	}
}

void ParallaxScrolling::draw(sf::RenderTarget & render, sf::RenderStates states) const
{
	for (auto const & layer : m_layers)
		layer->draw(render, states);
}
