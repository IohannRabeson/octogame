#ifndef CEDRICENDNPC_HPP
# define CEDRICENDNPC_HPP

# include "ANpc.hpp"
# include "ABiome.hpp"
# include "ChallengeManager.hpp"

class CedricEndNpc : public ANpc, public AGameObject<GameObjectType::CedricEndNpc>
{
public:
	CedricEndNpc(ABiome::Type biomeType);
	virtual ~CedricEndNpc(void) = default;

	void stopBalle(void);

	virtual void setup(void);
	virtual void update(sf::Time frametime);
	virtual float getHeight(void) const { return 0.f; }

protected:
	enum CedricEvents
	{
		IdleDay = Special2 + 1
	};

	virtual void setupMachine(void);
	virtual void updateState(void);

private:
	ChallengeManager::Effect	m_effect;
	octo::CharacterAnimation	m_idleDayAnimation;

};

#endif
