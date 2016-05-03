#ifndef BIRDBLUENPC_HPP
# define BIRDBLUENPC_HPP

# include "BirdNpc.hpp"

class BirdBlueNpc : public BirdNpc, public AGameObject<GameObjectType::BirdBlueNpc>
{
public:
	BirdBlueNpc(void);
	virtual ~BirdBlueNpc(void) = default;
};

#endif
