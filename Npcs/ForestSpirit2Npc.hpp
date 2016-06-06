#ifndef FORESTSPIRIT2NPC_HPP
# define FORESTSPIRIT2NPC_HPP

# include "DisappearNpc.hpp"

class ForestSpirit2Npc : public DisappearNpc, public AGameObject<GameObjectType::ForestSpirit2Npc>
{
public:
	ForestSpirit2Npc(void);
	virtual ~ForestSpirit2Npc(void) = default;
};

#endif
