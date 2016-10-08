#ifndef SMOKEINSTANCE_HPP
# define SMOKEINSTANCE_HPP

# include "SmokeSystem.hpp"
# include "InstanceDecor.hpp"
# include "AGameObject.hpp"

class CharacterOcto;
class RectangleShape;

class SmokeInstance : public InstanceDecor, public AGameObject<GameObjectType::SmokeInstance>
{
public:
	SmokeInstance(sf::Vector2f const & scale, sf::Vector2f const & position);
	virtual ~SmokeInstance(void) = default;

	void collideOctoEvent(CharacterOcto * octo);
	virtual void draw(sf::RenderTarget & render, sf::RenderStates states) const;
	virtual void update(sf::Time frametime);

	virtual void addMapOffset(float x, float y);
	virtual void setPosition(sf::Vector2f const & position);

private:
	RectangleShape *			m_box;
	SmokeSystem					m_smoke;
	sf::Vector2f				m_velocity;
};

#endif
