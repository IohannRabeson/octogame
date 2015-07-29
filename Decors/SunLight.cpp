#include "SunLight.hpp"
#include "ABiome.hpp"
#include "GameClock.hpp"
#include <Application.hpp>
#include <Camera.hpp>
#include <Interpolations.hpp>

SunLight::SunLight(void) :
	m_timerRain(sf::Time::Zero),
	m_timerRainMax(sf::seconds(2.f)),
	m_clock(nullptr)
{
}

SunLight::SunLight(GameClock * clock) :
	m_timerRain(sf::Time::Zero),
	m_timerRainMax(sf::seconds(2.f)),
	m_clock(clock)
{
}

void SunLight::createBicolorQuad(sf::Vector2f const & upLeft, sf::Vector2f const & upRight, sf::Vector2f const & downRight, sf::Vector2f const & downLeft, sf::Color const & colorUp, sf::Color const & colorDown, octo::VertexBuilder & builder)
{
	builder.createVertex(upLeft, colorUp);
	builder.createVertex(upRight, colorUp);
	builder.createVertex(downRight, colorDown);
	builder.createVertex(upLeft, colorUp);
	builder.createVertex(downLeft, colorDown);
	builder.createVertex(downRight, colorDown);
}

void SunLight::createSunLight(sf::Vector2f const & cameraSize, sf::Vector2f const & origin, octo::VertexBuilder& builder)
{
	sf::Vector2f nightUpLeft(-cameraSize.x, -cameraSize.y * 5);
	sf::Vector2f nightUpRight(cameraSize.x, -cameraSize.y * 5);
	sf::Vector2f nightDownLeft(-cameraSize.x, -cameraSize.y * 3);
	sf::Vector2f nightDownRight(cameraSize.x, -cameraSize.y * 3);

	sf::Vector2f sunsetLeft(-cameraSize.x, -cameraSize.y * 2);
	sf::Vector2f sunsetRight(cameraSize.x, -cameraSize.y * 2);

	sf::Vector2f dayUpLeft(-cameraSize.x, -cameraSize.y);
	sf::Vector2f dayUpRight(cameraSize.x, -cameraSize.y);
	sf::Vector2f dayDownLeft(-cameraSize.x, cameraSize.y * 2);
	sf::Vector2f dayDownRight(cameraSize.x, cameraSize.y * 2);

	nightUpLeft += origin;
	nightUpRight += origin;
	nightDownLeft += origin;
	nightDownRight += origin;

	sunsetLeft += origin;
	sunsetRight += origin;

	dayUpLeft += origin;
	dayUpRight += origin;
	dayDownLeft += origin;
	dayDownRight += origin;

	createBicolorQuad(dayUpLeft, dayUpRight, dayDownRight, dayDownLeft, m_colorDay, m_colorDay, builder);
	createBicolorQuad(sunsetLeft, sunsetRight, dayUpRight, dayUpLeft, m_colorSunset, m_colorDay, builder);
	createBicolorQuad(nightDownLeft, nightDownRight, sunsetRight, sunsetLeft, m_colorNight, m_colorSunset, builder);
	createBicolorQuad(nightUpLeft, nightUpRight, nightDownRight, nightDownLeft, m_colorNight, m_colorNight, builder);
}

void SunLight::setup(ABiome& biome)
{
	octo::Camera & camera = octo::Application::getCamera();
	m_cameraSize = camera.getSize();
	m_dayPos = sf::Vector2f(0.f, m_cameraSize.y);
	m_nightPos = sf::Vector2f(0.f, -m_cameraSize.y * 4.f);
	m_sunsetPos = sf::Vector2f(0.f, -m_cameraSize.y * 2.f);
	m_colorNight = biome.getNightLightColor();
	m_colorSunset = biome.getSunsetLightColor();
	m_colorDay = sf::Color::Transparent;
	m_colorDayRaining = sf::Color(100, 100, 100, 100);
}

void SunLight::computeDayColorValue(sf::Time frameTime, ABiome & biome)
{
	float weather = biome.getWeather();
	m_colorDayRaining.a = weather * 2.f;
	if (weather)
	{
		if (m_timerRain <= m_timerRainMax)
			m_timerRain += frameTime;
	}
	else
	{
		if (m_timerRain > sf::Time::Zero)
			m_timerRain -= frameTime;
	}
	m_colorDay = octo::linearInterpolation(sf::Color::Transparent, m_colorDayRaining, m_timerRain / m_timerRainMax);
}

void SunLight::update(sf::Time frameTime, octo::VertexBuilder& builder, ABiome& biome)
{
	sf::Vector2f position = getPosition();
	if (biome.canRain())
		computeDayColorValue(frameTime, biome);
	float dayValue = m_clock->getDayValue();
	float nightValue = m_clock->getNightValue();
	if (m_clock->isDay())
		position -= octo::linearInterpolation(m_sunsetPos, m_dayPos, dayValue);
	else if (m_clock->isNight())
		position -= octo::linearInterpolation(m_sunsetPos, m_nightPos, nightValue);
	createSunLight(m_cameraSize, position, builder);
}
