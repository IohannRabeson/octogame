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
		Wait,
		Appear,
		WalkToOcto,
		Speak,
		WalkToRocket,
		Stop
	};

	State		m_state;
};

#endif
