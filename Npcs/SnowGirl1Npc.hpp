#ifndef SNOWGIRL1NPC_HPP
# define SNOWGIRL1NPC_HPP

# include "ASpecialNpc.hpp"

class SnowGirl1Npc : public ASpecialNpc
{
public:
	SnowGirl1Npc(void);
	virtual ~SnowGirl1Npc(void) = default;

	virtual void setup(void);

};

#endif
