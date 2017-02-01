#ifndef BALLENANOROBOT_HPP
# define BALLENANOROBOT_HPP

# include "NanoRobot.hpp"
# include <CharacterSprite.hpp>

class BalleNanoRobot : public NanoRobot, public AGameObject<GameObjectType::BalleNanoRobot>
{
public:
	BalleNanoRobot(sf::Vector2f const & position);
	virtual ~BalleNanoRobot(void) = default;
	virtual void update(sf::Time frametime);

private:
	void updatePotion(sf::Time frametime);

	octo::CharacterSprite		m_potion;
	sf::Time					m_throwPotionTimer;
	sf::Time					m_throwPotionTimerMax;
	sf::Vector2f				m_octoPosition;
};

#endif
