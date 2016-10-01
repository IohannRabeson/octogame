#ifndef DESERTENGINE_HPP
# define DESERTENGINE_HPP

# include "AIdleNpc.hpp"
# include "RandomGenerator.hpp"

class DesertEngine : public AIdleNpc
{
public:
	DesertEngine(void);
	virtual ~DesertEngine(void) = default;

	virtual void setup(void);

private:
	void setSprite(void);

	RandomGenerator		m_generator;
};

#endif
