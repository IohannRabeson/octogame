#ifndef STRANGERSNOWNPC_HPP
# define STRANGERSNOWNPC_HPP

# include "ANpc.hpp"

class StrangerSnowNpc : public ANpc, public AGameObject<GameObjectType::StrangerSnowNpc>
{
public:
	StrangerSnowNpc(void);
	virtual ~StrangerSnowNpc(void) = default;

	virtual void setup(void);

protected:
	virtual void setupMachine(void);
	virtual void updateState(void);

};

#endif
