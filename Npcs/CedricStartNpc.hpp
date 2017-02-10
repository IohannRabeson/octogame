#ifndef CEDRICSTARTNPC_HPP
# define CEDRICSTARTNPC_HPP

# include "AUniqueNpc.hpp"
# include "ABiome.hpp"
# include "ChallengeManager.hpp"

class CedricStartNpc : public AUniqueNpc
{
public:
	CedricStartNpc(ABiome::Type biomeType);
	virtual ~CedricStartNpc(void) = default;

	bool startBalle(void);

	virtual void setup(void);
	virtual void collideOctoEvent(CharacterOcto * octo);
	virtual void update(sf::Time frametime);
	virtual void draw(sf::RenderTarget & render, sf::RenderStates states) const;
	virtual float getHeight(void) const { return 0.f; }

protected:
	enum CedricEvents
	{
		IdleNight = Special2 + 1,
		Special2Night
	};

	virtual void setupMachine(void);
	virtual void updateState(void);
	void		 updatePotion(sf::Time frameTime);

private:
	ChallengeManager::Effect	m_effect;
	octo::CharacterAnimation	m_idleNightAnimation;
	octo::CharacterAnimation	m_special2NightAnimation;
	octo::CharacterSprite		m_potion;
	sf::Time					m_throwPotionTimer;
	sf::Time					m_throwPotionTimerMax;
	sf::Vector2f				m_octoPosition;
};

#endif
