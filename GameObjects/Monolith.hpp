#ifndef MONOLITH_HPP
# define MONOLITH_HPP

# include "InstanceDecor.hpp"
# include "AGameObject.hpp"
# include <memory>

class MonolithStep;
class CircleShape;
class CharacterOcto;

class Monolith : public InstanceDecor, public AGameObject<GameObjectType::Monolith>
{
public:
	Monolith(sf::Vector2f const & scale, sf::Vector2f const & position);
	virtual ~Monolith(void) = default;

	virtual void addMapOffset(float x, float y);
	virtual void setPosition(sf::Vector2f const & position);

	void collideOcto(CharacterOcto * octo);
	virtual void update(sf::Time frameTime);
	virtual void draw(sf::RenderTarget& render, sf::RenderStates states) const;
	virtual void drawFront(sf::RenderTarget& render, sf::RenderStates states) const;

private:
	enum State
	{
		None,
		StartEffect,
		Activate,
		StartFinalScene,
	};
	octo::AnimatedSprite						m_spriteMonolith;
	octo::SpriteAnimation						m_animationMonolith;
	std::vector<std::unique_ptr<MonolithStep>>	m_steps;
	std::unique_ptr<MonolithStep>				m_monolith;
	CircleShape *								m_box;
	sf::Vector2f								m_position;
	sf::Time									m_timer;
	sf::Time									m_timerMax;
	State										m_state;
	float										m_offset;

};

#endif
