#ifndef AWALKNPC_HPP
# define AWALKNPC_HPP

# include "ANpc.hpp"

class AWalkNpc : public ANpc, public AGameObject<GameObjectType::WalkNpc>
{
public:
	AWalkNpc(ResourceKey const & npcId, bool isMeetable = true);
	virtual ~AWalkNpc(void) = default;

	virtual void setup(void) = 0;

protected:
	virtual void setupMachine(void);
	virtual void updateState(void);
	virtual void updatePhysics(void);

	void reverseWalking(bool reverse);
	float getVelocity(void) const;
	void setVelocity(float velocity);

	RandomGenerator		m_generator;
	std::size_t			m_lastState;
	float				m_velocity;
};

#endif
