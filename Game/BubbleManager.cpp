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
	std::string phrase = "Hello! Je suis Octo, un aventurier de l'espace. Je passe mon temps a visiter l'univers.";
	m_bubble1.setup(phrase, sf::Color(200, 0, 0, 200));

	phrase = "Salut Octo! Ravi de te rencontrer, tu as vraiment l'air super sympa comme Poulpe! Tu veux gouter une des boissons du coin?";
	m_bubble2.setup(phrase, sf::Color(255, 255, 255, 200));

	phrase = "WAAOUU!";
	m_bubble3.setup(phrase, sf::Color(200, 0, 0, 200), 80);
}

void BubbleManager::update(sf::Time frameTime)
{
	m_builder.clear();

	m_timer += frameTime.asSeconds();

	m_bubble1.update(frameTime, m_builder);
	m_bubble1.setPosition(sf::Vector2f(400.f, 500.f));

	if (m_timer > 4)
		m_bubble2.update(frameTime, m_builder);
	m_bubble2.setPosition(sf::Vector2f(700.f, 800.f));

	if (m_timer > 6)
		m_bubble3.update(frameTime, m_builder);
	m_bubble3.setPosition(sf::Vector2f(1200.f, 1000.f));

	m_used = m_builder.getUsed();
}

void BubbleManager::draw(sf::RenderTarget & render, sf::RenderStates states) const
{
	render.draw(m_vertices.get(), m_used, sf::Triangles, states);
	render.draw(m_bubble1.getText());
	render.draw(m_bubble2.getText());
	render.draw(m_bubble3.getText());
}

