#ifndef CHALLENGEMANAGER_HPP
# define CHALLENGEMANAGER_HPP

# include <SFML/Graphics/Shader.hpp>
# include <SFML/System/Time.hpp>
# include <map>
# include <memory>
# include "ResourceDefinitions.hpp"

class ChallengeManager
{
public:
	enum class Effect : std::size_t
	{
		Pixelate = 0u,
		Persistence = 1u,
		Duplicate = 2u,
		Displacement = 3u
	};

	class Challenge
	{
	public:
		Challenge(ResourceKey key, float challengeDuration, float glitchDuration);
		virtual ~Challenge(void) = default;

		virtual void update(sf::Time frametime) = 0;
		void start(void);
		void stop(void);
		bool enable(void) const;
		bool isFinished(void) const;
		sf::Time getDuration(void) const;
		void setDuration(float duration);
		// reminissence

	protected:
		sf::Shader		m_shader;
		std::size_t		m_index;
		sf::Time		m_timer;
		sf::Time		m_duration;
		sf::Time		m_challengeDuration;
		sf::Time		m_glitchDuration;
	};

	~ChallengeManager(void) = default;
	static ChallengeManager & getInstance(void);

	Challenge & getEffect(Effect effect);

	void reset(void);
	void update(sf::Time frametime);

private:
	static std::unique_ptr<ChallengeManager>		m_instance;
	std::map<Effect, std::unique_ptr<Challenge>>	m_challenges;

	ChallengeManager(void);
	void addEffect(ResourceKey key, Effect effect);

};

class ChallengeDuplicate : public ChallengeManager::Challenge
{
public:
	ChallengeDuplicate(void);
	virtual void update(sf::Time frametime);

private:
	sf::Time	m_delay;
};

#endif
