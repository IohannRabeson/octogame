#ifndef CHALLENGEMANAGER_HPP
# define CHALLENGEMANAGER_HPP

# include <SFML/Graphics/Shader.hpp>
# include <SFML/System/Time.hpp>
# include <map>
# include <memory>
# include "ResourceDefinitions.hpp"
# include "ABiome.hpp"
# include "RandomGenerator.hpp"

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
		Challenge(ResourceKey key, float challengeDuration, float glitchDuration, sf::FloatRect const & area, ABiome::Type biomeType);
		virtual ~Challenge(void) = default;

		virtual void updateShader(sf::Time frametime) = 0;
		void updateGlitch(sf::Time frametime);
		void updateChallenge(sf::Time frametime);
		void start(void);
		void stop(void);
		bool enable(void) const;
		bool isFinished(void) const;
		bool isGlitch(void) const;
		sf::Time getDuration(void) const;
		ABiome::Type getBiomeType(void) const;
		void setDuration(float duration);
		virtual void setGlitch(bool isGlitch);
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
		ABiome::Type	m_biomeType;
		bool			m_validArea;
		bool			m_isGlitch;
	};

	~ChallengeManager(void) = default;
	static ChallengeManager & getInstance(void);

	Challenge & getEffect(Effect effect);

	void reset(void);
	void update(ABiome & biome, sf::Vector2f const & position, sf::Time frametime);

private:
	static std::unique_ptr<ChallengeManager>		m_instance;
	std::map<Effect, std::unique_ptr<Challenge>>	m_challenges;
	RandomGenerator									m_generator;
	sf::Time										m_glitchTimer;

	ChallengeManager(void);
	void addEffect(ResourceKey key, Effect effect);

};

class ChallengeDuplicate : public ChallengeManager::Challenge
{
public:
	ChallengeDuplicate(void);
	virtual void updateShader(sf::Time frametime);

	virtual void setGlitch(bool isGlitch);

private:
	float	m_rotation;
	float	m_intensity;
};

#endif
