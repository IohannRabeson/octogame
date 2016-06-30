#ifndef ROCKET_HPP
# define ROCKET_HPP

# include "ANpc.hpp"
# include "SmokeSystem.hpp"

class Rocket : public ANpc, public AGameObject<GameObjectType::Rocket>
{
public:
	Rocket(void);
	virtual ~Rocket(void) = default;

	virtual void setup(void);
	virtual void update(sf::Time frametime);
	virtual void draw(sf::RenderTarget & render, sf::RenderStates states) const;

protected:
	virtual void setupMachine(void);
	virtual void updateTimer(sf::Time frametime);
	virtual void updateState(void);
	virtual void updatePhysics(void);

	std::size_t						m_smokesCount;
	std::unique_ptr<SmokeSystem[]>	m_smokes;
	bool							m_collide;
	sf::Time						m_timerBefore;
	sf::Time						m_timerBeforeMax;
	sf::Time						m_timerFirstBlast;
	sf::Time						m_timerFirstBlastMax;
	sf::Time						m_timerSecondBlast;
	sf::Time						m_timerSecondBlastMax;
	sf::Vector2f					m_position;
};

#endif
