#ifndef AFISHNPC_HPP
# define AFISHNPC_HPP

# include "ASwimNpc.hpp"

class AFishNpc : public ASwimNpc
{
public:
	AFishNpc(ResourceKey const & npcId, ABiome & biome, bool isMeetable);
	virtual ~AFishNpc(void) = default;

	virtual void setup(void) = 0;

protected:
	virtual void update(sf::Time frametime);
};

#endif
