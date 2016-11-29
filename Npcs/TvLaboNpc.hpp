#ifndef TVLABONPC_HPP
# define TVLABONPC_HPP

# include "ANpc.hpp"
# include "RandomGenerator.hpp"

class TvLaboNpc : public ANpc
{
public:
	TvLaboNpc(ResourceKey key);
	virtual ~TvLaboNpc(void) = default;

	virtual void setup(void);

protected:
	virtual void setupMachine(void);
	virtual void updateState(void);

private:
	RandomGenerator		m_generator;
};

#endif
