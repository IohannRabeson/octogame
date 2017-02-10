#ifndef SNOWMAN1NPC_HPP
# define SNOWMAN1NPC_HPP

# include "ASpecialNpc.hpp"

class Snowman1Npc : public ASpecialNpc
{
public:
	Snowman1Npc(void);
	virtual ~Snowman1Npc(void) = default;

	virtual void setup(void);

};

#endif
