#include "BubbleManager.hpp"

#include <Application.hpp>
#include <ResourceManager.hpp>
#include "ResourceDefinitions.hpp"

BubbleManager::BubbleManager(void) :
	m_vertices(new sf::Vertex[10000]),
	m_count(10000),
	m_used(0u)
{
	m_builder = octo::VertexBuilder(m_vertices.get(), m_count);
}

//TODO: Add NPC array in setup
void BubbleManager::setup()//(std::vector<NPC> npc)
{
	octo::ResourceManager const & resource = octo::Application::getResourceManager();
	std::size_t bubbleCount = 1;//npc.size;
	m_bubbles.resize(bubbleCount);

	for (auto &bubble : m_bubbles)
	{
		std::string phrase = resource.getText(NPC_TEST_TXT);
		bubble.setup(phrase, sf::Color(255, 255, 255, 200));
		bubble.setActive(false);
	}
}

void BubbleManager::update(sf::Time frameTime, sf::Vector2f const & octoPos)
{
	m_builder.clear();
	m_timer += frameTime;

	for (auto &bubble : m_bubbles)
	{
		bubble.update(frameTime, m_builder);
		bubble.setPosition(octoPos);
		if (m_timer >= sf::seconds(7.f))
			bubble.setActive(true);
	}

	m_used = m_builder.getUsed();
}

void BubbleManager::draw(sf::RenderTarget & render, sf::RenderStates states) const
{
	render.draw(m_vertices.get(), m_used, sf::Triangles, states);
	for (auto bubble : m_bubbles)
	{
		//TODO: Potion idea, put sf::Blendultiply on tex, it create illisible effect
		if (bubble.isActive())
			render.draw(bubble.getText());
	}
}

