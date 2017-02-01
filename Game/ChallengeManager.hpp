#ifndef CHALLENGEMANAGER_HPP
# define CHALLENGEMANAGER_HPP

# include <SFML/System/Time.hpp>
# include <map>
# include <memory>
# include "ResourceDefinitions.hpp"
# include "RandomGenerator.hpp"
# include "ABiome.hpp"

namespace sf
{
	class Shader;
}

class ChallengeManager
{
public:
	class AChallenge
	{
	public:
		AChallenge(ResourceKey key, float challengeDuration, float delay, sf::FloatRect const & area, ABiome::Type biomeType, std::pair<float, float> glitchIntensityRange, std::pair<float, float> glitchDurationRange);
		virtual ~AChallenge(void);

		void update(sf::Time frametime, ABiome & biome, sf::Vector2f const & position);
		void updateGlitch(sf::Time frametime, ABiome & biome);
		void updateGlitchManual(sf::Time frametime, ABiome & biome);
		void updateChallenge(sf::Time frametime);
		void updatePitch(float maxVariation);
		virtual void updateShader(sf::Time frametime) = 0;

		void start(void);
		void stop(void);
		bool enable(void) const;

		bool isFinished(void) const;
		bool isGlitch(void) const;
		void startGlitch(ABiome & biome);
		void startGlitchManual(ABiome & biome);
		sf::Time getDuration(void) const;
		ABiome::Type getBiomeType(void) const;
		void setDuration(float duration);
		void setIntensity(float intensity);
		void setGlitch(bool isGlitch);
		void validateArea(ABiome & biome, sf::Vector2f const & position);

	protected:
		sf::Shader &					m_shader;
		std::size_t						m_index;
		sf::Time						m_delay;
		sf::Time						m_delayMax;
		sf::Time						m_timer;
		sf::Time						m_duration;
		sf::Time						m_glitchTimer;
		sf::FloatRect					m_area;
		ABiome::Type					m_biomeType;
		float							m_intensity;
		bool							m_validArea;
		bool							m_isGlitch;
		std::pair<float, float>			m_glitchIntensityRange;
		std::pair<float, float>			m_glitchDurationRange;
	};

	enum class Effect : std::size_t
	{
		Pixelate = 1u,
		Persistence = 2u,
		Duplicate = 4u,
		Displacement = 8u,
		Blur = 16u
	};

	~ChallengeManager(void);
	static ChallengeManager & getInstance(void);

	AChallenge & getEffect(Effect effect);

	void reset(void);
	void update(ABiome & biome, sf::Vector2f const & position, sf::Time frametime);
	bool launchManualGlitch(bool isLaunch);

private:
	static std::unique_ptr<ChallengeManager>		m_instance;
	std::map<Effect, std::unique_ptr<AChallenge>>	m_challenges;
	bool											m_launchManualGlitch;
	bool											m_lockManualGlitch;

	ChallengeManager(void);
	void addEffect(ResourceKey key, Effect effect);

	void updateNormal(ABiome & biome, sf::Vector2f const & position, sf::Time frametime);
	void updateRandom(ABiome & biome, sf::Vector2f const & position, sf::Time frametime);
	void updateManualGlitch(ABiome & biome, sf::Time frametime);
};

#endif
