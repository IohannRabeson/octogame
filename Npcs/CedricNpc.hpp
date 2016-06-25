#ifndef CEDRICNPC_HPP
# define CEDRICNPC_HPP

# include "ANpc.hpp"
# include "ABiome.hpp"
# include "ChallengeManager.hpp"

class CedricNpc : public ANpc, public AGameObject<GameObjectType::CedricNpc>
{
public:
	CedricNpc(ABiome::Type biomeType);
	virtual ~CedricNpc(void);

	bool startBalle(void);

	virtual void setup(void);
	virtual void update(sf::Time frametime);
	virtual float getHeight(void) const { return 0.f; }

protected:
	enum CedricEvents
	{
		Special2Night,
	};

	virtual void setupMachine(void);
	virtual void updateState(void);

private:
	ChallengeManager::Effect	m_effect;

};

#endif
