#ifndef ANPCSPECIAL_HPP
# define ANPCSPECIAL_HPP

# include "ANpc.hpp"

class ASpecialNpc : public ANpc, public AGameObject<GameObjectType::SpecialNpc>
{
public:
	ASpecialNpc(ResourceKey const & npcId, bool followOcto = true, bool isMeetable = true);
	virtual ~ASpecialNpc(void) = default;

	virtual void setup(void) = 0;
	virtual void update(sf::Time frameTime);

protected:
	virtual void setupMachine(void);
	virtual void updateState(void);

private:
	bool		m_canDoSpecial;
};

#endif
