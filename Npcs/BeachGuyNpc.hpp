#ifndef BEACHGUYNPC_HPP
# define BEACHGUYNPC_HPP

# include "ASpecialNpc.hpp"

class BeachGuyNpc : public ASpecialNpc
{
public:
	BeachGuyNpc(void);
	virtual ~BeachGuyNpc(void) = default;

	virtual void setup(void);

};

#endif
