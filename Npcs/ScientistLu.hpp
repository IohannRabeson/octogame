#ifndef SCIENTISTLU_HPP
# define SCIENTISTLU_HPP

# include "ANpc.hpp"

class ScientistLu : public ANpc, public AGameObject<GameObjectType::ScientistLu>
{
public:
	ScientistLu(void);
	virtual ~ScientistLu(void) = default;

	virtual void setup(void);

protected:
	virtual void setupMachine(void);
	virtual void updateState(void);

};

#endif
