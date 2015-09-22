#ifndef IPLACEABLE_HPP
# define IPLACEABLE_HPP

# include <SFML/System/Vector2.hpp>

class IPlaceable
{
public:
	virtual ~IPlaceable(void) {}

	virtual void setPosition(sf::Vector2f const & position) = 0;
	virtual void update(sf::Time frametime) = 0;
	virtual void draw(sf::RenderTarget &, sf::RenderStates) const {}
	virtual void drawFront(sf::RenderTarget &, sf::RenderStates) const {}

private:

};

#endif
