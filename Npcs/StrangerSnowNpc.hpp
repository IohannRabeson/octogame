#ifndef STRANGERSNOWNPC_HPP
# define STRANGERSNOWNPC_HPP

# include "ASpecialNpc.hpp"

class StrangerSnowNpc : public ASpecialNpc
{
public:
	StrangerSnowNpc(void);
	virtual ~StrangerSnowNpc(void) = default;

	virtual void setup(void);

};

#endif
