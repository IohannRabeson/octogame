#include "SunLight.hpp"
#include "ABiome.hpp"
#include "GameClock.hpp"
#include <Application.hpp>
#include <Camera.hpp>
#include <Interpolations.hpp>

SunLight::SunLight(void) :
	m_clock(nullptr)
{
}

SunLight::SunLight(GameClock * clock) :
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

	nightUpLeft += origin;
	nightUpRight += origin;
	nightDownLeft += origin;
	nightDownRight += origin;

	sunsetLeft += origin;
	sunsetRight += origin;

	dayUpLeft += origin;
	dayUpRight += origin;

	createBicolorQuad(sunsetLeft, sunsetRight, dayUpRight, dayUpLeft, m_colorSunset, sf::Color::Transparent, builder);
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
}

void SunLight::update(sf::Time, octo::VertexBuilder& builder, ABiome&)
{
	sf::Vector2f position = getPosition();
	float dayValue = m_clock->getDayValue();
	float nightValue = m_clock->getNightValue();
	if (m_clock->isDay())
		position -= octo::linearInterpolation(m_sunsetPos, m_dayPos, dayValue);
	else if (m_clock->isNight())
		position -= octo::linearInterpolation(m_sunsetPos, m_nightPos, nightValue);
	createSunLight(m_cameraSize, position, builder);
}
