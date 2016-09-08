#ifndef CLOUD_HPP
# define CLOUD_HPP

# include "ADecor.hpp"
# include "DecorAnimator.hpp"
# include "DropSystem.hpp"
# include "Lightning.hpp"
# include <AudioManager.hpp>
# include <SFML/Graphics/Color.hpp>

class SkyCycle;

class Cloud : public ADecor
{
struct OctogonValue
{
	sf::Vector2f	size;
	sf::Vector2f	sizeCorner;
	sf::Vector2f	origin;
};

public:
	Cloud(void);
	Cloud(SkyCycle * cycle);
	virtual ~Cloud(void);

	virtual void setup(ABiome& biome);
	virtual void update(sf::Time frameTime,
						octo::VertexBuilder& builder,
						ABiome& biome);

	virtual bool isDisabledIfOutOfScreen()const;

private:
	void createOctogon(sf::Vector2f const & size,
						sf::Vector2f const & sizeCorner,
						sf::Vector2f const & origin,
						sf::Color color,
						octo::VertexBuilder& builder);

	bool isOctogonContain(sf::Vector2f const & size,
							sf::Vector2f const & position,
							sf::Vector2f const & point) const;

	void createCloud(std::vector<OctogonValue> const & values,
					sf::Vector2f const & origin,
					std::size_t partCount,
					sf::Color const & color,
					octo::VertexBuilder& builder);

	void setupLightning(ABiome & biome);

	void newCloud(ABiome & biome);

	void updateThunder(sf::Time frameTime,
						ABiome & biome,
						octo::VertexBuilder & builder,
						sf::Vector2f const & position);

	void updateRain(sf::Time frameTime,
					ABiome & biome,
					octo::VertexBuilder & builder,
					sf::Vector2f const & position,
					float weather);

	void updateSnow(sf::Time frameTime,
					ABiome & biome,
					octo::VertexBuilder & builder,
					sf::Vector2f const & position,
					float weather);

	void updatePosition(sf::Time frameTime);

	sf::Vector2f					m_position;
	sf::Vector2f					m_size;
	std::size_t						m_partCount;
	sf::Vector2f					m_speed;
	bool							m_isSpecialCloud;
	float							m_cloudMinX;
	float							m_cloudMaxX;
	float							m_cloudMinY;
	float							m_cloudMaxY;
	float							m_cos;
	float							m_sin;
	sf::Color						m_color;
	std::vector<OctogonValue>		m_values;

	DecorAnimator					m_animator;
	float							m_animation;
	bool							m_isCollide;

	bool							m_canWeather;
	std::vector<DropSystem *>		m_rain;
	std::vector<DropSystem *>		m_snow;
	std::vector<sf::Vector2f>		m_dropUpLeft;

	bool							m_thunderCloud;
	Lightning						m_lightning;
	float							m_lightningSize;
	sf::Vector2f					m_p0;
	sf::Vector2f					m_p1;

	octo::AudioManager::SoundPtr	m_rainSound;
	SkyCycle *						m_cycle;
};

#endif
