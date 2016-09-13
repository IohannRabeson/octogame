#ifndef SCIENTISTOCTO_HPP
# define SCIENTISTOCTO_HPP

# include "ANpc.hpp"

class ScientistOcto : public ANpc
{
public:
	ScientistOcto(void);
	virtual ~ScientistOcto(void) = default;

	virtual void setup(void);

protected:
	virtual void setupMachine(void);
	virtual void updateState(void);

};

#endif
