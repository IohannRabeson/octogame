#ifndef FAUST_HPP
# define FAUST_HPP

# include "ASpecialNpc.hpp"

class FaustNpc : public ASpecialNpc
{
public:
	FaustNpc(void);
	virtual ~FaustNpc(void) = default;

	virtual void setup(void);

};

#endif
