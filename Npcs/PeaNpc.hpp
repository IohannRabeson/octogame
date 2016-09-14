#ifndef PEA_HPP
# define PEA_HPP

# include "ASpecialNpc.hpp"

class PeaNpc : public ASpecialNpc
{
public:
	PeaNpc(void);
	virtual ~PeaNpc(void) = default;

	virtual void setup(void);

};

#endif
