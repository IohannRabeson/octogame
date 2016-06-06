#ifndef FORESTSPIRIT1NPC_HPP
# define FORESTSPIRIT1NPC_HPP

# include "DisappearNpc.hpp"

class ForestSpirit1Npc : public DisappearNpc, public AGameObject<GameObjectType::ForestSpirit1Npc>
{
public:
	ForestSpirit1Npc(void);
	virtual ~ForestSpirit1Npc(void) = default;
};

#endif
