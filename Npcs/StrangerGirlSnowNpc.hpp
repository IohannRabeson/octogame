#ifndef STRANGERGIRLSNOWNPC_HPP
# define STRANGERGIRLSNOWNPC_HPP

# include "ANpc.hpp"

class StrangerGirlSnowNpc : public ANpc, public AGameObject<GameObjectType::StrangerGirlSnowNpc>
{
public:
	StrangerGirlSnowNpc(void);
	virtual ~StrangerGirlSnowNpc(void) = default;

	virtual void setup(void);

protected:
	virtual void setupMachine(void);
	virtual void updateState(void);

};

#endif
