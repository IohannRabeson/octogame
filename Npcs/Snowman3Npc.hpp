#ifndef SNOWMAN3NPC_HPP
# define SNOWMAN3NPC_HPP

# include "ASpecialNpc.hpp"

class Snowman3Npc : public ASpecialNpc
{
public:
	Snowman3Npc(void);
	virtual ~Snowman3Npc(void) = default;

	virtual void setup(void);

};

#endif
