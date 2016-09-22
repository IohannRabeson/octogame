#ifndef ENGINESNOW_HPP
# define ENGINESNOW_HPP

# include "AIdleNpc.hpp"
# include "RandomGenerator.hpp"

class EngineSnow : public AIdleNpc
{
public:
	EngineSnow(void);
	virtual ~EngineSnow(void) = default;

	virtual void setup(void);

private:
	RandomGenerator		m_generator;
};

#endif
