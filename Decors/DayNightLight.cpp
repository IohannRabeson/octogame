#include "DayNightLight.hpp"
#include "ABiome.hpp"
#include "GameClock.hpp"
#include <Application.hpp>
#include <Camera.hpp>
#include <Interpolations.hpp>

DayNightLight::DayNightLight(void) :
	m_clock(nullptr)
{
}

DayNightLight::DayNightLight(GameClock * clock) :
	m_clock(clock)
{
}

void DayNightLight::createBicolorQuad(sf::Vector2f const & upLeft, sf::Vector2f const & upRight, sf::Vector2f const & downRight, sf::Vector2f const & downLeft, sf::Color const & colorUp, sf::Color const & colorDown, octo::VertexBuilder & builder)
{
	builder.createVertex(upLeft, colorUp);
	builder.createVertex(upRight, colorUp);
	builder.createVertex(downRight, colorDown);
	builder.createVertex(upLeft, colorUp);
	builder.createVertex(downLeft, colorDown);
	builder.createVertex(downRight, colorDown);
}

void DayNightLight::createDayNightLight(sf::Vector2f const & cameraSize, sf::Vector2f const & origin, octo::VertexBuilder& builder)
{
	sf::Vector2f nightUpLeft(-cameraSize.x, -cameraSize.y * 5);
	sf::Vector2f nightUpRight(cameraSize.x, -cameraSize.y * 5);
	sf::Vector2f nightDownLeft(-cameraSize.x, -cameraSize.y * 3);
	sf::Vector2f nightDownRight(cameraSize.x, -cameraSize.y * 3);

	sf::Vector2f middleLeft(-cameraSize.x, -cameraSize.y * 2);
	sf::Vector2f middleRight(cameraSize.x, -cameraSize.y * 2);

	sf::Vector2f dayUpLeft(-cameraSize.x, -cameraSize.y);
	sf::Vector2f dayUpRight(cameraSize.x, -cameraSize.y);
	sf::Vector2f dayDownLeft(-cameraSize.x, cameraSize.y);
	sf::Vector2f dayDownRight(cameraSize.x, cameraSize.y);

	nightUpLeft += origin;
	nightUpRight += origin;
	nightDownLeft += origin;
	nightDownRight += origin;

	middleLeft += origin;
	middleRight += origin;

	dayUpLeft += origin;
	dayUpRight += origin;
	dayDownLeft += origin;
	dayDownRight += origin;

//	createBicolorQuad(dayUpLeft, dayUpRight, dayDownRight, dayDownLeft, m_transparancy, m_transparancy, builder);
	createBicolorQuad(middleLeft, middleRight, dayUpRight, dayUpLeft, m_colorMiddle, m_transparancy, builder);
	createBicolorQuad(nightDownLeft, nightDownRight, middleRight, middleLeft, m_colorNight, m_colorMiddle, builder);
	createBicolorQuad(nightUpLeft, nightUpRight, nightDownRight, nightDownLeft, m_colorNight, m_colorNight, builder);
}

void DayNightLight::setup(ABiome& biome)
{
	(void)biome;
	octo::Camera & camera = octo::Application::getCamera();
	m_cameraSize = camera.getSize();
	m_dayPos = sf::Vector2f(0.f, m_cameraSize.y);
	m_nightPos = sf::Vector2f(0.f, -m_cameraSize.y * 4.f);
	m_middlePos = sf::Vector2f(0.f, -m_cameraSize.y * 2.f);
	m_transparancy = sf::Color(255, 255, 255, 0);
	m_colorDay = sf::Color(255, 244, 0, 100);//biome.getDayNightLightColor();
	m_colorNight = sf::Color(0, 197, 255, 100);//biome.getDayNightLightColor();
	m_colorMiddle = sf::Color(238, 173, 181, 100);//biome.getDayNightLightColor();
}

void DayNightLight::update(sf::Time frameTime, octo::VertexBuilder& builder, ABiome&)
{
	(void)frameTime;
	sf::Vector2f position = getPosition();
	float dayValue = m_clock->getDayValue();
	float nightValue = m_clock->getNightValue();
	if (m_clock->isDay())
		position -= octo::linearInterpolation(m_middlePos, m_dayPos, dayValue);
	else if (m_clock->isNight())
		position -= octo::linearInterpolation(m_middlePos, m_nightPos, nightValue);
	createDayNightLight(m_cameraSize, position, builder);
}
