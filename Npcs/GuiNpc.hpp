#ifndef GUINPC_HPP
# define GUINPC_HPP

# include "AWalkNpc.hpp"

class GuiNpc : public AWalkNpc
{
public:
	GuiNpc(void);
	virtual ~GuiNpc(void) = default;

	virtual void setup(void);

};

#endif
