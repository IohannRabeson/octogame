#ifndef GROUNDTRANSFORMNANOROBOT_HPP
# define GROUNDTRANSFORMNANOROBOT_HPP

# include "NanoRobot.hpp"

class GroundTransformNanoRobot : public NanoRobot, public AGameObject<GameObjectType::GroundTransformNanoRobot>
{
public:
	GroundTransformNanoRobot(void);
	virtual ~GroundTransformNanoRobot(void) = default;

	virtual void update(sf::Time frameTime);
	virtual void drawText(sf::RenderTarget & render, sf::RenderStates states) const;

private:
	sf::Time	m_textTimer;
	sf::Time	m_textTimerMax;
	bool		m_canSpeak;
	bool		m_firstSpawn;

};

#endif
