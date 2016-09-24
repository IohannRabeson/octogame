#ifndef ASINKNPC_HPP
# define ASINKNPC_HPP

# include "ANpc.hpp"
# include "SmokeSystem.hpp"

class ASinkNpc : public ANpc, public AGameObject<GameObjectType::SinkNpc>
{
public:
	ASinkNpc(ResourceKey const & npcId, float waterLevel, sf::Color waterColor);
	virtual ~ASinkNpc(void) = default;

	virtual void			setup(void) = 0;
	virtual void 			update(sf::Time frametime);
	virtual void 			draw(sf::RenderTarget & render, sf::RenderStates states) const;

protected:
	virtual void 			setupMachine(void);
	virtual void 			updatePhysics(void);
	virtual void 			updateState(void);

	bool					isCollide(void);
	SmokeSystem &			getSmoke(void);
	float					getWaterLevel(void);

private:
	void					setupSmoke(void);
	void					updateTimer(sf::Time frametime);

	bool					m_isCollide;
	float					m_waterLevel;
	sf::Color				m_waterColor;
	bool					m_fallingWater;
	SmokeSystem				m_smoke;
	sf::Time				m_timerFloating;
	sf::Time				m_timerFloatingMax;
};

#endif
