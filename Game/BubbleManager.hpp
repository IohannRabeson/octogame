#ifndef BUBBLEMANAGER_HPP
# define BUBBLEMANAGER_HPP

#include "ABubble.hpp"

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
	//TODO:: Add explicit in front of constructor
	BubbleManager(std::size_t maxVertexCount);
	~BubbleManager(void) = default;

	void setup(void);
	void update(sf::Time frameTime);
	virtual void draw(sf::RenderTarget& render, sf::RenderStates states) const;

private:
	std::unique_ptr<sf::Vertex[]>	m_vertices;
	std::size_t						m_count;
	std::size_t						m_used;
	octo::VertexBuilder				m_builder;

	ABubble							m_bubble;
};

#endif
