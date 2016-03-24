#ifndef SNOWMAN1NPC_HPP
# define SNOWMAN1NPC_HPP

# include "ANpc.hpp"

class Snowman1Npc : public ANpc, public AGameObject<GameObjectType::Snowman1Npc>
{
public:
	Snowman1Npc(void);
	virtual ~Snowman1Npc(void) = default;

	virtual void setup(void);

protected:
	virtual void setupMachine(void);
	virtual void updateState(void);

};

#endif
