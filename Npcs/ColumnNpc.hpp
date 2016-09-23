#ifndef COLUMNNPC_HPP
# define COLUMNNPC_HPP

# include "AIdleNpc.hpp"
# include "RandomGenerator.hpp"

class ColumnNpc : public AIdleNpc
{
public:
	ColumnNpc(void);
	virtual ~ColumnNpc(void) = default;

	virtual void setup(void);

private:
	void setSprite(void);

	RandomGenerator		m_generator;
};

#endif
