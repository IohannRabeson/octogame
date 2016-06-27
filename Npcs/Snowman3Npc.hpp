#ifndef SNOWMAN3NPC_HPP
# define SNOWMAN3NPC_HPP

# include "ANpc.hpp"

class Snowman3Npc : public ANpc, public AGameObject<GameObjectType::Snowman3Npc>
{
public:
	Snowman3Npc(void);
	virtual ~Snowman3Npc(void);

	virtual void setup(void);

protected:
	virtual void setupMachine(void);
	virtual void updateState(void);

};

#endif
