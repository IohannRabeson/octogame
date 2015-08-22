#ifndef BUBBLEMANAGER_HPP
# define BUBBLEMANAGER_HPP

#include "BubbleNPC.hpp"

//TODO: Check if all are necessary
# include <VertexBuilder.hpp>
# include <SFML/Graphics/Vertex.hpp>
# include <SFML/Graphics/RenderTarget.hpp>
# include <SFML/Graphics/Drawable.hpp>
# include <SFML/Graphics/Transformable.hpp>
# include <SFML/System/Time.hpp>

class BubbleManager : public sf::Drawable,
					  public sf::Transformable
{
public:
	BubbleManager(std::size_t maxVertexCount);
	~BubbleManager(void) = default;

	void setup(void);
	void update(sf::Time frameTime, sf::Vector2f const & octoPos);
	virtual void draw(sf::RenderTarget& render, sf::RenderStates states) const;

private:
	std::unique_ptr<sf::Vertex[]>	m_vertices;
	std::size_t						m_count;
	std::size_t						m_used;
	octo::VertexBuilder				m_builder;

	std::vector<BubbleNPC>			m_bubbles;
	sf::Time						m_timer;
};

#endif
