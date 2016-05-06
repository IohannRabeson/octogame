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
	void		 updateInfo(void);

private:
	enum State
	{
		FirstSpawn,
		Walk,
		MoveMap,
		TalkAboutShit,
		None
	};

	sf::Time	m_textTimer;
	sf::Time	m_textTimerMax;
	bool		m_canSpeak;
	bool		m_talkaboutshit;
	State		m_state;
	std::size_t	m_npcCount;
	std::size_t m_npcMax;
	sf::Time	m_timerDemo;
	sf::Time	m_timerDemoMax;
};

#endif
