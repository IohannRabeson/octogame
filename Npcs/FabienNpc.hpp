#ifndef FABIENNPC_HPP
# define FABIENNPC_HPP

# include "BirdNpc.hpp"

class FabienNpc : public BirdNpc, public AGameObject<GameObjectType::FabienNpc>
{
public:
	FabienNpc(void);
	virtual ~FabienNpc(void) = default;
	virtual void setup(void);
};

#endif
