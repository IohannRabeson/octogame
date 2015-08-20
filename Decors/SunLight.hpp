#ifndef SUNLIGHT_HPP
# define SUNLIGHT_HPP

# include "ADecor.hpp"
# include "DecorAnimator.hpp"

# include <VertexBuilder.hpp>
# include <SFML/Graphics/Vertex.hpp>
# include <SFML/Graphics/RenderTarget.hpp>
# include <SFML/Graphics/Drawable.hpp>
# include <SFML/Graphics/Transformable.hpp>
# include <SFML/System/Time.hpp>
# include <SFML/Graphics/Color.hpp>
# include <SFML/Graphics/RenderTexture.hpp>
# include <SFML/Graphics/Texture.hpp>
# include <SFML/Graphics/Sprite.hpp>

class SkyCycle;

class SunLight : public sf::Drawable, public sf::Transformable
{
public:
	SunLight(void);
	virtual ~SunLight(void) = default;

	virtual void setup(ABiome& biome, SkyCycle & skyCycle);
	virtual void update(sf::Time frameTime);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

private:
	void createSunLight(sf::Vector2f const & size,
						sf::Vector2f const & origin,
						sf::Color const & colorDay,
						sf::Color const & colorSunset,
						sf::Color const & colorNight,
						octo::VertexBuilder& builder);

	void createBicolorQuad(sf::Vector2f const & upLeft,
							sf::Vector2f const & upRight,
							sf::Vector2f const & downRight,
							sf::Vector2f const & downLeft,
							sf::Color const & colorUp,
							sf::Color const & colorDown,
							octo::VertexBuilder & builder);

	void computeDayColorValue(sf::Time frameTime);

	void setupLights(void);
	void updateLights(void);

	struct Light
	{
		sf::Vector2f	position;
		sf::Vector2f	scale;
		sf::Color		color;

		Light(void) :
			scale(1.f, 1.f),
			color(255, 255, 255, 255)
		{
		}

		Light(sf::Vector2f position, sf::Vector2f scale, sf::Color color) :
			position(position),
			scale(scale),
			color(color)
		{
		}
	};

	std::unique_ptr<sf::Vertex[]>	m_vertices;
	std::size_t						m_count;
	std::size_t						m_used;
	octo::VertexBuilder				m_builder;

	sf::Vector2f					m_size;
	sf::Vector2f					m_cameraSize;
	sf::Vector2f					m_dayPos;
	sf::Vector2f					m_nightPos;
	sf::Vector2f					m_sunsetPos;
	sf::Color						m_colorNight;
	sf::Color						m_colorSunset;
	sf::Color						m_colorDay;
	sf::Color						m_colorDayRaining;
	bool							m_canCreateRain;

	sf::Time						m_timerRain;
	sf::Time						m_timerRainMax;
	DecorAnimator					m_animator;
	float							m_animation;

	sf::Texture						m_lightTexture;
	sf::Sprite						m_lightSprite;
	sf::RenderTexture				m_lightMapTexture;
	sf::Sprite						m_lightMap;
	std::vector<Light>				m_rotateLights;

	SkyCycle *						m_cycle;

private:
	static void rotateVec(sf::Vector2f & vector, float const cosAngle, float const sinAngle);
	static void rotateVec(sf::Vector2f & vector, sf::Vector2f const & origin, float const cosAngle, float const sinAngle);
};

#endif
