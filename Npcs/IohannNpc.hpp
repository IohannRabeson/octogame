#ifndef IOHANNNPC_HPP
# define IOHANNNPC_HPP

# include "AWalkNpc.hpp"

class IohannNpc : public AWalkNpc
{
public:
	IohannNpc(void);
	virtual ~IohannNpc(void) = default;

	virtual void setup(void);
};

#endif
