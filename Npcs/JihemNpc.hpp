#ifndef JIHEMNPC_HPP
# define JIHEMNPC_HPP

# include "ASpecialNpc.hpp"

class JihemNpc : public ASpecialNpc
{
public:
	JihemNpc(void);
	virtual ~JihemNpc(void) = default;

	virtual void setup(void);

};

#endif
