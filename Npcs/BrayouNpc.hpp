#ifndef BRAYOU_HPP
# define BRAYOU_HPP

# include "ANpc.hpp"

class BrayouNpc : public ANpc, public AGameObject<GameObjectType::BrayouNpc>
{
public:
	BrayouNpc(void);
	virtual ~BrayouNpc(void) = default;

	virtual void setup(void);

protected:
	virtual void setupMachine(void);
	virtual void updateState(void);

};

#endif
