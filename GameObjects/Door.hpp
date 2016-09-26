#ifndef DOOR_HPP
# define DOOR_HPP

# include "InstanceDecor.hpp"
# include "AGameObject.hpp"

class RectangleShape;

class Door : public InstanceDecor, public AGameObject<GameObjectType::Door>
{
public:
	Door(sf::Vector2f const & scale, sf::Vector2f const & position);
	virtual ~Door(void) = default;

	virtual void draw(sf::RenderTarget & render, sf::RenderStates states) const;
	virtual void update(sf::Time frametime);

	virtual void addMapOffset(float x, float y);
	virtual void setPosition(sf::Vector2f const & position);

private:
	RectangleShape *				m_box;

};

#endif
