#ifndef SCIENTISTFRAN_HPP
# define SCIENTISTFRAN_HPP

# include "ANpc.hpp"

class ScientistFran : public ANpc, public AGameObject<GameObjectType::ScientistFran>
{
public:
	ScientistFran(void);
	virtual ~ScientistFran(void) = default;

	virtual void setup(void);

protected:
	virtual void setupMachine(void);
	virtual void updateState(void);

};

#endif
