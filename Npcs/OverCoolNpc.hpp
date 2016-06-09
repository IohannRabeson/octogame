#ifndef OVERCOOLNPC_HPP
# define OVERCOOLNPC_HPP

# include "BirdNpc.hpp"

class OverCoolNpc : public BirdNpc, public AGameObject<GameObjectType::OverCoolNpc>
{
public:
	OverCoolNpc(void);
	virtual ~OverCoolNpc(void) = default;
	virtual void setup(void);
};

#endif
