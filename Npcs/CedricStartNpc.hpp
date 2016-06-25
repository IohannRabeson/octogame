#ifndef CEDRICSTARTNPC_HPP
# define CEDRICSTARTNPC_HPP

# include "ANpc.hpp"
# include "ABiome.hpp"
# include "ChallengeManager.hpp"

class CedricStartNpc : public ANpc, public AGameObject<GameObjectType::CedricStartNpc>
{
public:
	CedricStartNpc(ABiome::Type biomeType);
	virtual ~CedricStartNpc(void) = default;

	bool startBalle(void);

	virtual void setup(void);
	virtual void update(sf::Time frametime);
	virtual float getHeight(void) const { return 0.f; }

protected:
	enum CedricEvents
	{
		IdleNight = Special2 + 1,
		Special2Night
	};

	virtual void setupMachine(void);
	virtual void updateState(void);

private:
	ChallengeManager::Effect	m_effect;
	octo::CharacterAnimation	m_idleNightAnimation;
	octo::CharacterAnimation	m_special2NightAnimation;

};

#endif
