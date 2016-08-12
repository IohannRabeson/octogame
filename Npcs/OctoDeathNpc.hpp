#ifndef OCTODEATHNPC_HPP
# define OCTODEATHNPC_HPP

# include "ANpc.hpp"
# include "SmokeSystem.hpp"

class OctoDeathNpc : public ANpc, public AGameObject<GameObjectType::OctoDeathNpc>
{
public:
	OctoDeathNpc(float waterLevel, sf::Color waterColor);
	virtual ~OctoDeathNpc(void) = default;

	virtual void setup(void);
	virtual void update(sf::Time frametime);
	virtual void draw(sf::RenderTarget & render, sf::RenderStates states) const;

protected:
	virtual void setupMachine(void);
	virtual void updatePhysics(void);
	virtual void updateState(void);

private:
	bool			m_isCollide;
	float			m_waterLevel;
	sf::Color		m_waterColor;
	bool			m_fallingWater;
	SmokeSystem		m_smoke;
};

#endif
