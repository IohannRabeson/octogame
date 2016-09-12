#ifndef CAVEMANNPC_HPP
# define CAVEMANNPC_HPP

# include "ANpcSpecial.hpp"

class CavemanNpc : public ANpcSpecial
{
public:
	CavemanNpc(void);
	virtual ~CavemanNpc(void) = default;

	virtual void setup(void);

private:

};

#endif
