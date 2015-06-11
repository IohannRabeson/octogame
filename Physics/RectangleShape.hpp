#ifndef RECTANGLESHAPE_HPP
# define RECTANGLESHAPE_HPP

# include "PolygonShape.hpp"

class RectangleShape : public PolygonShape
{
public:
	RectangleShape(void);
	virtual ~RectangleShape(void) = default;

	void setSize(sf::Vector2f const & size);
	sf::Vector2f const & getSize(void) const;

private:
	sf::Vector2f	m_size;

};

#endif
