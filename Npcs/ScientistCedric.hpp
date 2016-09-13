#ifndef SCIENTISTCEDRIC_HPP
# define SCIENTISTCEDRIC_HPP

# include "ScientistNpc.hpp"

class ScientistCedric : public ScientistNpc, public AGameObject<GameObjectType::ScientistCedric>
{
public:
	ScientistCedric(void);
	virtual ~ScientistCedric(void) = default;

	virtual void setup(void);

protected:
	virtual void setupMachine(void);
	virtual void updateState(void);
};

#endif
