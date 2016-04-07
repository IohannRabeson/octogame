#ifndef SNOWMAN2NPC_HPP
# define SNOWMAN2NPC_HPP

# include "ANpc.hpp"

class Snowman2Npc : public ANpc, public AGameObject<GameObjectType::Snowman2Npc>
{
public:
	Snowman2Npc(void);
	virtual ~Snowman2Npc(void) = default;

	virtual void setup(void);

protected:
	virtual void setupMachine(void);
	virtual void updateState(void);

};

#endif
