#ifndef SNOWMAN2NPC_HPP
# define SNOWMAN2NPC_HPP

# include "ASpecialNpc.hpp"

class Snowman2Npc : public ASpecialNpc
{
public:
	Snowman2Npc(void);
	virtual ~Snowman2Npc(void) = default;

	virtual void setup(void);

};

#endif
