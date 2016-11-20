#ifndef SKELETONNPC_HPP
# define SKELETONNPC_HPP

# include "AIdleNpc.hpp"
# include "RandomGenerator.hpp"

class SkeletonNpc : public AIdleNpc
{
public:
	SkeletonNpc(void);
	virtual ~SkeletonNpc(void) = default;

	virtual void setup(void);

private:
	void setSprite(void);

	RandomGenerator		m_generator;
};

#endif
