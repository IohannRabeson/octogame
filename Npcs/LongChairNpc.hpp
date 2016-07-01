#ifndef LONGCHAIRNPC_HPP
# define LONGCHAIRNPC_HPP

# include "ANpc.hpp"

class LongChairNpc : public ANpc, public AGameObject<GameObjectType::LongChairNpc>
{
public:
	LongChairNpc(void);
	virtual ~LongChairNpc(void) = default;

	virtual void setup(void);

protected:
	virtual void setupMachine(void);
	virtual void updateState(void);

};

#endif
