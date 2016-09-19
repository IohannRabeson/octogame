#ifndef FORESTSPIRIT1NPC_HPP
# define FORESTSPIRIT1NPC_HPP

# include "ADisappearNpc.hpp"

class ForestSpirit1Npc : public ADisappearNpc
{
public:
	ForestSpirit1Npc(void);
	virtual ~ForestSpirit1Npc(void) = default;

	virtual void setup(void);

};

#endif
