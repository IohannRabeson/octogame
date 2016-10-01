#ifndef DEEPO_HPP
# define DEEPO_HPP

# include "AFlyNpc.hpp"

class Deepo : public AFlyNpc
{
public:
	Deepo(void);
	virtual ~Deepo(void) = default;

	virtual void setup(void);

};

#endif
