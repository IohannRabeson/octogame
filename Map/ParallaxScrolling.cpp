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

// Nested class ALayer
ParallaxScrolling::ALayer::ALayer(void) :
	m_offset(0.f),
	m_speed(0.5f),
	m_isScrolling(true)
{}

void ParallaxScrolling::ALayer::updateOffset(float offset)
{
	m_offset = offset * m_speed;
}
