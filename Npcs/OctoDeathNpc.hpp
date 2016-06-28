#ifndef OCTODEATHNPC_HPP
# define OCTODEATHNPC_HPP

# include "ANpc.hpp"

class OctoDeathNpc : public ANpc, public AGameObject<GameObjectType::OctoDeathNpc>
{
public:
	OctoDeathNpc(void);
	virtual ~OctoDeathNpc(void) = default;

	virtual void setup(void);
	virtual void update(sf::Time frametime);

protected:
	virtual void setupMachine(void);
	virtual void updateState(void);

private:
	bool m_isCollide;
};

#endif
