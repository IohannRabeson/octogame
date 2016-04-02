#ifndef SNOWGIRL1NPC_HPP
# define SNOWGIRL1NPC_HPP

# include "ANpc.hpp"

class SnowGirl1Npc : public ANpc, public AGameObject<GameObjectType::SnowGirl1Npc>
{
public:
	SnowGirl1Npc(void);
	virtual ~SnowGirl1Npc(void) = default;

	virtual void setup(void);

protected:
	virtual void setupMachine(void);
	virtual void updateState(void);

};

#endif
