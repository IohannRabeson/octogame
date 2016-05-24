#ifndef CHALLENGEMANAGER_HPP
# define CHALLENGEMANAGER_HPP

# include <SFML/Graphics/Shader.hpp>
# include <SFML/System/Time.hpp>
# include <map>
# include <memory>
# include "ResourceDefinitions.hpp"

class ABiome;

class ChallengeManager
{
public:
	enum class Effect : std::size_t
	{
		Pixelate = 0u,
		Persistence = 1u,
		Duplicate = 2u,
		Displacement = 4u
	};

	class Challenge
	{
	public:
		Challenge(ResourceKey key, float challengeDuration, float glitchDuration, sf::FloatRect const & area);
		virtual ~Challenge(void) = default;

		virtual void updateShader(sf::Time frametime) = 0;
		void update(sf::Time frametime);
		void start(void);
		void stop(void);
		bool enable(void) const;
		bool isFinished(void) const;
		sf::Time getDuration(void) const;
		void setDuration(float duration);
		void validateArea(ABiome & biome, sf::Vector2f const & position);
		// reminissence

	protected:
		sf::Shader		m_shader;
		std::size_t		m_index;
		sf::Time		m_delay;
		sf::Time		m_delayMax;
		sf::Time		m_timer;
		sf::Time		m_duration;
		sf::Time		m_challengeDuration;
		sf::Time		m_glitchDuration;
		sf::FloatRect	m_area;
		bool			m_validArea;
	};

	~ChallengeManager(void) = default;
	static ChallengeManager & getInstance(void);

	Challenge & getEffect(Effect effect);

	void reset(void);
	void update(ABiome & biome, sf::Vector2f const & position, sf::Time frametime);

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
	virtual void updateShader(sf::Time frametime);
};

#endif
