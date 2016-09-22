#ifndef STRANGERGIRLSNOWNPC_HPP
# define STRANGERGIRLSNOWNPC_HPP

# include "ASpecialNpc.hpp"

class StrangerGirlSnowNpc : public ASpecialNpc
{
public:
	StrangerGirlSnowNpc(void);
	virtual ~StrangerGirlSnowNpc(void) = default;

	virtual void setup(void);

};

#endif
