#ifndef TVFANNPC_HPP
# define TVFANNPC_HPP

# include "ASpecialNpc.hpp"

class TVFanNpc : public ASpecialNpc
{
public:
	TVFanNpc(void);
	virtual ~TVFanNpc(void) = default;

	virtual void setup(void);

};

#endif
