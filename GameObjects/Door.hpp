#ifndef DOOR_HPP
# define DOOR_HPP

# include "InstanceDecor.hpp"
# include "AGameObject.hpp"

class RectangleShape;
class CharacterOcto;
class SkyCycle;

class Door : public InstanceDecor, public AGameObject<GameObjectType::Door>
{
public:
	Door(SkyCycle & sckyCycle, sf::Vector2f const & scale, sf::Vector2f const & position);
	virtual ~Door(void) = default;

	void collideOcto(CharacterOcto * octo);
	virtual void draw(sf::RenderTarget & render, sf::RenderStates states) const;
	virtual void update(sf::Time frametime);

	virtual void addMapOffset(float x, float y);
	virtual void setPosition(sf::Vector2f const & position);

private:
	bool							m_lock;
	RectangleShape *				m_box;
	SkyCycle &						m_skyCycle;
	sf::Time						m_timerSpeed;
	sf::Time						m_timerSpeedMax;
	bool							m_actionEnable;

};

#endif
