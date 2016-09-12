#ifndef ANPCSPECIAL_HPP
# define ANPCSPECIAL_HPP

# include "ANpc.hpp"

class ANpcSpecial : public ANpc, public AGameObject<GameObjectType::SpecialNpc>
{
public:
	ANpcSpecial(ResourceKey const & npcId, bool isMeetable = true);
	virtual ~ANpcSpecial(void) = default;

	virtual void setup(void) = 0;
	virtual void update(sf::Time frameTime);

protected:
	virtual void setupMachine(void);
	virtual void updateState(void);

private:
	bool		m_canDoSpecial;

};

#endif
