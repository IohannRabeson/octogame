#include "BubbleManager.hpp"

#include <Application.hpp>
#include <ResourceManager.hpp>
#include "ResourceDefinitions.hpp"

BubbleManager::BubbleManager(std::size_t maxVertexCount) :
	m_vertices(new sf::Vertex[maxVertexCount]),
	m_count(maxVertexCount),
	m_used(0u)
{
	m_builder = octo::VertexBuilder(m_vertices.get(), m_count);
}

//TODO: Add NPC array in setup
void BubbleManager::setup(void)//(std::vector<NPC> npc)
{
	octo::ResourceManager const & resource = octo::Application::getResourceManager();
	std::size_t bubbleCount = 3;//npc.size;
	m_bubbles.resize(bubbleCount);

	for (auto bubble : m_bubbles)
	{
		std::string phrase = resource.getText(NPC_TEST_TXT);
		bubble.setup(phrase, sf::Color(200, 0, 0, 200));
		bubble.setActive(true);
	}
		std::string phrase = resource.getText(NPC_TEST_TXT);
		bubble.setup(phrase, sf::Color(200, 0, 0, 200));
		bubble.setActive(true);
}

void BubbleManager::update(sf::Time frameTime)
{
	m_builder.clear();

	m_timer += frameTime.asSeconds();

	for (auto bubble : m_bubbles)
	{
		bubble.update(frameTime, m_builder);
		bubble.setPosition(sf::Vector2f(400.f, 500.f));
	}
		bubble.update(frameTime, m_builder);
		bubble.setPosition(sf::Vector2f(400.f, 500.f));

	m_used = m_builder.getUsed();
}

void BubbleManager::draw(sf::RenderTarget & render, sf::RenderStates states) const
{
	render.draw(m_vertices.get(), m_used, sf::Triangles, states);
	for (auto bubble : m_bubbles)
	{
		if (bubble.isActive())
			render.draw(bubble.getText());
	}
		if (bubble.isActive())
			render.draw(bubble.getText());
}

