#ifndef WOLFNPC_HPP
# define WOLFNPC_HPP

# include "ANpc.hpp"

class WolfNpc : public ANpc, public AGameObject<GameObjectType::WolfNpc>
{
public:
	WolfNpc(void);
	virtual ~WolfNpc(void) = default;

	virtual void setup(void);

protected:
	virtual void setupMachine(void);
	virtual void updateState(void);
	virtual void updatePhysics(void);
	void setWalkEvent(std::size_t event);

	std::mt19937								m_engine;
	std::uniform_int_distribution<std::size_t>	m_specialDistribution;
	std::size_t									m_lastState;
	float										m_velocity;

};

#endif
