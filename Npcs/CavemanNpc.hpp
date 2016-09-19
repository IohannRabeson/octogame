#ifndef CAVEMANNPC_HPP
# define CAVEMANNPC_HPP

# include "ASpecialNpc.hpp"

class CavemanNpc : public ASpecialNpc
{
public:
	CavemanNpc(void);
	virtual ~CavemanNpc(void) = default;

	virtual void setup(void);

};

#endif
