#ifndef BIRDREDNPC_HPP
# define BIRDREDNPC_HPP

# include "BirdNpc.hpp"

class BirdRedNpc : public BirdNpc, public AGameObject<GameObjectType::BirdRedNpc>
{
public:
	BirdRedNpc(void);
	virtual ~BirdRedNpc(void) = default;
};

#endif
