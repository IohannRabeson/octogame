#ifndef PIERRE_HPP
# define PIERRE_HPP

# include "ANpc.hpp"

class PierreNpc : public ANpc, public AGameObject<GameObjectType::PierreNpc>
{
public:
	PierreNpc(void);
	virtual ~PierreNpc(void) = default;

	virtual void setup(void);

protected:
	virtual void setupMachine(void);
	virtual void updateState(void);

};

#endif
