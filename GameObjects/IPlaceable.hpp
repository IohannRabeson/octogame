#ifndef IPLACEABLE_HPP
# define IPLACEABLE_HPP

# include <SFML/System/Vector2.hpp>

class IPlaceable
{
public:
	virtual ~IPlaceable(void) {}

	virtual void setPosition(sf::Vector2f const & position) = 0;
	virtual void update(sf::Time frametime) = 0;
	virtual void draw(sf::RenderTarget & render, sf::RenderStates states) const = 0;

private:

};

#endif
