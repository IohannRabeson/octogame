#ifndef MARYVONNENPC_HPP
# define MARYVONNENPC_HPP

# include "AIdleNpc.hpp"

class MaryvonneNpc : public AIdleNpc
{
public:
	MaryvonneNpc(void);
	virtual ~MaryvonneNpc(void) = default;

	virtual void setup(void);

};

#endif
