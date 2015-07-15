#ifndef GENERATIVELAYER_HPP
# define GENERATIVELAYER_HPP

# include <memory>
# include "Noise.hpp"
# include "ParallaxScrolling.hpp"

class GenerativeLayer : public ParallaxScrolling::ALayer
{
public:
	GenerativeLayer(void) :
		ALayer(0.5f)
	{
		m_vertices.reset(new sf::Vertex[120 * 4]);
		m_positions.reset(new sf::Vector2f[120 * 4]);
		m_noise.setSeed(42);

		m_positions[0u] = sf::Vector2f(0.f, m_noise.fBm(0.f, 5.f, 3, 3.f, 0.3f) * 128.f);
		m_positions[1u] = sf::Vector2f(16.f, m_noise.fBm(0.f, 5.f, 3, 3.f, 0.3f) * 128.f);
		m_positions[2u] = sf::Vector2f(16.f, 100.f);
		m_positions[3u] = sf::Vector2f(0.f, 100.f);
		for (std::size_t i = 4u; i < 120 - 1u; i++)
		{
			m_positions[(i * 4u) + 0u] = sf::Vector2f(i * 16.f, m_positions[(i * 4u)  - 3u].y);
			m_positions[(i * 4u) + 1u] = sf::Vector2f((i + 1u) * 16.f, m_noise.fBm((i * 4u) / 100.f, 5.f, 3, 3.f, 0.3f) * 128.f);
			m_positions[(i * 4u) + 2u] = sf::Vector2f((i + 1u) * 16.f, 100.f);
			m_positions[(i * 4u) + 3u] = sf::Vector2f(i * 16.f, 100.f);

			m_vertices[(i * 4u) + 0u].position = m_positions[(i * 4u)  + 0u];
			m_vertices[(i * 4u) + 0u].color = sf::Color::Blue;
			m_vertices[(i * 4u) + 1u].position = m_positions[(i * 4u)  + 1u];
			m_vertices[(i * 4u) + 1u].color = sf::Color::Blue;
			m_vertices[(i * 4u) + 2u].position = m_positions[(i * 4u)  + 2u];
			m_vertices[(i * 4u) + 2u].color = sf::Color::Blue;
			m_vertices[(i * 4u) + 3u].position = m_positions[(i * 4u) + 3u];
			m_vertices[(i * 4u) + 3u].color = sf::Color::Blue;
		}
	}
	virtual ~GenerativeLayer(void) = default;

	void update(float)
	{
		for (std::size_t i = 0u; i < 120; i++)
		{
			m_vertices[(i * 4u) + 0u].position.x = getOffset() + m_positions[(i * 4u) + 0u].x;
			m_vertices[(i * 4u) + 1u].position.x = getOffset() + m_positions[(i * 4u) + 1u].x;
			m_vertices[(i * 4u) + 2u].position.x = getOffset() + m_positions[(i * 4u) + 2u].x;
			m_vertices[(i * 4u) + 3u].position.x = getOffset() + m_positions[(i * 4u) + 3u].x;
		}
	}

	void draw(sf::RenderTarget & render, sf::RenderStates states) const
	{
		render.draw(m_vertices.get(), 120 * 4u, sf::Quads, states);
	}

private:
	std::unique_ptr<sf::Vertex []>		m_vertices;
	std::unique_ptr<sf::Vector2f []>	m_positions;
	Noise								m_noise;

};

#endif
