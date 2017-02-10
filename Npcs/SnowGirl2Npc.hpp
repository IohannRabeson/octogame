#ifndef SNOWGIRL2NPC_HPP
# define SNOWGIRL2NPC_HPP

# include "ASpecialNpc.hpp"

class SnowGirl2Npc : public ASpecialNpc
{
public:
	SnowGirl2Npc(void);
	virtual ~SnowGirl2Npc(void) = default;

	virtual void setup(void);

};

#endif
