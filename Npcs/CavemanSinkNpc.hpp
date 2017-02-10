#ifndef CAVEMANSINKNPC_HPP
# define CAVEMANSINKNPC_HPP

# include "ASinkNpc.hpp"
# include "SmokeSystem.hpp"

class CavemanSinkNpc : public ASinkNpc
{
public:
	CavemanSinkNpc(float waterLevel, sf::Color waterColor);
	virtual ~CavemanSinkNpc(void) = default;

	virtual void setup(void);
	virtual void update(sf::Time frametime);

protected:
	virtual void updatePhysics(void);

private:
	bool			m_fallingWater;
	sf::Time		m_startSinkTimer;
	sf::Time		m_startSinkTimerMax;
	sf::Time		m_fadeTimer;
	sf::Time		m_fadeTimerMax;
};

#endif
