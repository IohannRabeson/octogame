#ifndef AIDLENPC_HPP
# define AIDLENPC_HPP

# include "ANpc.hpp"

class AIdleNpc : public ANpc, public AGameObject<GameObjectType::IdleNpc>
{
public:
	AIdleNpc(ResourceKey const & npcId, bool followOcto = true, bool isMeetable = true);
	virtual ~AIdleNpc(void) = default;

	virtual void setup(void) = 0;

protected:
	virtual void setupMachine(void);
	virtual void updateState(void);
};

#endif
