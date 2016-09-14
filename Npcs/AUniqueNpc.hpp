#ifndef UNIQUENPC_HPP
# define UNIQUENPC_HPP

# include "ANpc.hpp"

class AUniqueNpc : public ANpc, public AGameObject<GameObjectType::Npc>
{
public:
	AUniqueNpc(ResourceKey const & npcId, bool isMeetable = true);
	virtual ~AUniqueNpc(void) = default;

	virtual void setup(void) = 0;

protected:
	virtual void setupMachine(void) = 0;
	virtual void updateState(void) = 0;

};

#endif
