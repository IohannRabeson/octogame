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
	virtual void draw(sf::RenderTarget & render, sf::RenderStates states) const;
	virtual void transfertToOcto(bool inInit = false);
	virtual void drawText(sf::RenderTarget & render, sf::RenderStates states) const;

	bool throwPotion(bool isPotion);

private:
	void updatePotion(sf::Time frametime);

	octo::CharacterSprite		m_potion;
	bool						m_isThrowPotion;
	sf::Time					m_throwPotionTimer;
	sf::Time					m_throwPotionTimerMax;
	sf::Vector2f				m_octoPosition;
	bool						m_inInit;
};

#endif
