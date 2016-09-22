#ifndef OCTODEATHNPC_HPP
# define OCTODEATHNPC_HPP

# include "ASinkNpc.hpp"
# include "SmokeSystem.hpp"

class OctoDeathNpc : public ASinkNpc
{
public:
	OctoDeathNpc(float waterLevel, sf::Color waterColor);
	virtual ~OctoDeathNpc(void) = default;

	virtual void setup(void);
};

#endif
