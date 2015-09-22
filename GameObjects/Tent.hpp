#ifndef TENT_HPP
# define TENT_HPP

# include "SimpleObject.hpp"

class Tent : public SimpleObject, public AGameObject<GameObjectType::Tent>
{
public:
	Tent(void);
	virtual ~Tent(void);

	virtual void update(sf::Time frameTime);
	virtual void setPosition(sf::Vector2f const & position);
	virtual void draw(sf::RenderTarget &, sf::RenderStates) const;
	virtual void drawFront(sf::RenderTarget &, sf::RenderStates) const;

};

#endif
