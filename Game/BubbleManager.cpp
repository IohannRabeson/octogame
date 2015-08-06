#include "BubbleManager.hpp"

BubbleManager::BubbleManager(std::size_t maxVertexCount) :
	m_vertices(new sf::Vertex[maxVertexCount]),
	m_count(maxVertexCount),
	m_used(0u)
{
	m_builder = octo::VertexBuilder(m_vertices.get(), m_count);
}

void BubbleManager::setup(void)
{
	m_bubble.setup();
}

void BubbleManager::update(sf::Time frameTime)
{
	m_builder.clear();

	m_bubble.update(frameTime, m_builder);

	m_used = m_builder.getUsed();
}

void BubbleManager::draw(sf::RenderTarget & render, sf::RenderStates states) const
{
	render.draw(m_vertices.get(), m_used, sf::Triangles, states);
	render.draw(m_bubble.getText());
}

