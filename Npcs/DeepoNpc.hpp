#ifndef DEEPONPC_HPP
# define DEEPONPC_HPP

# include "AFlyNpc.hpp"

class DeepoNpc : public AFlyNpc
{
public:
	DeepoNpc(void);
	virtual ~DeepoNpc(void) = default;

	virtual void setup(void);

};

#endif
