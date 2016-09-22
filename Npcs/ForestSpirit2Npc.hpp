#ifndef FORESTSPIRIT2NPC_HPP
# define FORESTSPIRIT2NPC_HPP

# include "ADisappearNpc.hpp"

class ForestSpirit2Npc : public ADisappearNpc
{
public:
	ForestSpirit2Npc(void);
	virtual ~ForestSpirit2Npc(void) = default;

	virtual void setup(void);
};

#endif
