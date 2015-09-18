#ifndef TENT_HPP
# define TENT_HPP

# include "AGameObject.hpp"
# include "SimpleObject.hpp"

class Tent : public SimpleObject, public AGameObject<GameObjectType::Tent>
{
public:
	Tent(void);
	virtual ~Tent(void);

	virtual void setPosition(sf::Vector2f const & position);
};

#endif
