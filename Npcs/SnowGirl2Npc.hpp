#ifndef SNOWGIRL2NPC_HPP
# define SNOWGIRL2NPC_HPP

# include "ANpc.hpp"

class SnowGirl2Npc : public ANpc, public AGameObject<GameObjectType::SnowGirl2Npc>
{
public:
	SnowGirl2Npc(void);
	virtual ~SnowGirl2Npc(void) = default;

	virtual void setup(void);

protected:
	virtual void setupMachine(void);
	virtual void updateState(void);

};

#endif
