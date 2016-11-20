#ifndef CEDRICWALKNPC_HPP
# define CEDRICWALKNPC_HPP

# include "AWalkNpc.hpp"

class CedricWalkNpc : public AWalkNpc
{
public:
	CedricWalkNpc(void);
	virtual ~CedricWalkNpc(void) = default;

	virtual void setup(void);
	virtual void update(sf::Time frametime);
	virtual void updateState(void);
	virtual void collideOctoEvent(CharacterOcto * octo);

private:
	enum State
	{
		Appear,
		WalkToOcto,
		Speak,
		WalkToRocket,
		Stop,
		Wait,
		FollowOcto
	};

	State		m_state;
	sf::Time	m_timerSpeak;
	sf::Time	m_timerWalkToRocket;
	sf::Time	m_drinkSoundTimer;
};

#endif
