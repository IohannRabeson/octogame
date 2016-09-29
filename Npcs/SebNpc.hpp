#ifndef SEBNPC_HPP
# define SEBNPC_HPP

# include "ASwimNpc.hpp"

class SebNpc : public ASwimNpc
{
public:
	SebNpc(ABiome & biome);
	virtual ~SebNpc(void) = default;

	virtual void setup(void);
	virtual void update(sf::Time frameTime);
};

#endif
