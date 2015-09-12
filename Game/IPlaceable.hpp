#ifndef IPLACEABLE_HPP
# define IPLACEABLE_HPP

# include <SFML/System/Vector2.hpp>

class IPlaceable
{
public:
	virtual void setPosition(sf::Vector2f const & position) = 0;

private:

};

#endif
