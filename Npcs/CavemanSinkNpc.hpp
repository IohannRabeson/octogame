#ifndef CAVEMANSINKNPC_HPP
# define CAVEMANSINKNPC_HPP

# include "ASinkNpc.hpp"
# include "SmokeSystem.hpp"

class CavemanSinkNpc : public ASinkNpc
{
public:
	CavemanSinkNpc(float waterLevel, sf::Color waterColor);
	virtual ~CavemanSinkNpc(void) = default;

	virtual void setup(void);
};

#endif
