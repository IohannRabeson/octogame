#ifndef VINCE_HPP
# define VINCE_HPP

# include "AIdleNpc.hpp"

class VinceNpc : public AIdleNpc
{
public:
	VinceNpc(void);
	virtual ~VinceNpc(void) = default;

	virtual void setup(void);

};

#endif
