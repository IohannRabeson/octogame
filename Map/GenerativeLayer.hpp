#ifndef GENERATIVELAYER_HPP
# define GENERATIVELAYER_HPP

# include <memory>
# include "Noise.hpp"
# include "ParallaxScrolling.hpp"

class GenerativeLayer : public ParallaxScrolling::ALayer
{
public:
	GenerativeLayer(void) :
		ALayer(0.5f),
		m_width(512u), //get from biom
		m_height(128u),
		m_widthScreen(122u)
	{
		m_vertices.reset(new sf::Vertex[m_width * 4]);
		m_positions.reset(new sf::Vector2f[m_width * 4]);
		m_noise.setSeed(42);

		m_positions[0u] = sf::Vector2f(0.f, getHeightValue(0u));//m_noise.fBm(0.f, 5.f, 3, 3.f, 0.3f) * 128.f);
		m_positions[1u] = sf::Vector2f(16.f, getHeightValue(0u));//m_noise.fBm(0.f, 5.f, 3, 3.f, 0.3f) * 128.f);
		m_positions[2u] = sf::Vector2f(16.f, static_cast<float>(m_height) * 12.f);
		m_positions[3u] = sf::Vector2f(0.f, static_cast<float>(m_height) * 12.f);
		for (std::size_t i = 4u; i < m_width - 1u; i++)
		{
			m_positions[(i * 4u) + 0u] = sf::Vector2f(i * 16.f, m_positions[(i * 4u)  - 3u].y);
			m_positions[(i * 4u) + 1u] = sf::Vector2f((i + 1u) * 16.f, getHeightValue(i * 4u));//(m_noise.fBm((i * 4u) / 100.f, 5.f, 3, 3.f, 0.3f) + 1.f) / 2.f * static_cast<float>(m_height) * 12.f);
			m_positions[(i * 4u) + 2u] = sf::Vector2f((i + 1u) * 16.f, static_cast<float>(m_height) * 12.f);
			m_positions[(i * 4u) + 3u] = sf::Vector2f(i * 16.f, static_cast<float>(m_height) * 12.f);

			m_vertices[(i * 4u) + 0u].position = m_positions[(i * 4u)  + 0u];
			m_vertices[(i * 4u) + 0u].color = sf::Color(100, 100, 100);
			m_vertices[(i * 4u) + 1u].position = m_positions[(i * 4u)  + 1u];
			m_vertices[(i * 4u) + 1u].color = sf::Color(100, 100, 100);
			m_vertices[(i * 4u) + 2u].position = m_positions[(i * 4u)  + 2u];
			m_vertices[(i * 4u) + 2u].color = sf::Color(100, 100, 100);
			m_vertices[(i * 4u) + 3u].position = m_positions[(i * 4u) + 3u];
			m_vertices[(i * 4u) + 3u].color = sf::Color(100, 100, 100);
		}
	}
	virtual ~GenerativeLayer(void) = default;

	float getHeightValue(int x)
	{
		float f = (m_noise.fBm(static_cast<float>(x) / 100.f, 5.f, 3, 3.f, 0.3f) + 1.f) / 2.f * static_cast<float>(m_height) * 12.f;
		int value = static_cast<int>(f) % 16;
		return f - value;
	}

	void update(float)
	{
		//std::size_t index = getOffset() / 16.f;
		for (std::size_t i = 0u; i < m_widthScreen; i++)
		{
			m_vertices[(i * 4u) + 0u].position.x = getOffset() + m_positions[(i * 4u) + 0u].x;
			m_vertices[(i * 4u) + 1u].position.x = getOffset() + m_positions[(i * 4u) + 1u].x;
			m_vertices[(i * 4u) + 2u].position.x = getOffset() + m_positions[(i * 4u) + 2u].x;
			m_vertices[(i * 4u) + 3u].position.x = getOffset() + m_positions[(i * 4u) + 3u].x;
		}
	}

	void draw(sf::RenderTarget & render, sf::RenderStates states) const
	{
		render.draw(m_vertices.get(), m_width * 4u, sf::Quads, states);
	}

private:
	std::unique_ptr<sf::Vertex []>		m_vertices;
	std::unique_ptr<sf::Vector2f []>	m_positions;
	Noise								m_noise;
	std::size_t							m_width;
	std::size_t							m_height;
	std::size_t							m_widthScreen;

};

#endif
