#ifndef CHRISTIANNPC_HPP
# define CHRISTIANNPC_HPP

# include "ASpecialNpc.hpp"
# include "SmokeSystem.hpp"

class ChristianNpc : public ASpecialNpc
{
public:
	ChristianNpc(void);
	virtual ~ChristianNpc(void) = default;

	virtual void setup(void);
	virtual void update(sf::Time frametime);
	virtual void draw(sf::RenderTarget & render, sf::RenderStates states) const;

private:
	sf::Vector2f	m_smokePosition;
	sf::Vector2f	m_puffPosition;
	SmokeSystem		m_smoke;
	SmokeSystem		m_puff;
	sf::Time		m_puffTimer;
	sf::Time		m_puffTimerMin;
	sf::Time		m_puffTimerMax;
};

#endif
