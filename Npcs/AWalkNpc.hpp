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

};

#endif
