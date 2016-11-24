#ifndef CEDRICICEANPC_HPP
# define CEDRICICEANPC_HPP

# include "ASpecialNpc.hpp"

class CedricIceANpc : public ASpecialNpc
{
public:
	CedricIceANpc(void);
	virtual ~CedricIceANpc(void) = default;

	virtual void setup(void);
	virtual void update(sf::Time frametime);

private:
	sf::Time	m_timerMove;
	sf::Time	m_timerMoveMax;
	sf::Time	m_timerDisapear;
	sf::Time	m_timerDisapearMax;
};

#endif
