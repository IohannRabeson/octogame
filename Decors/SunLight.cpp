#include "SunLight.hpp"
#include "ABiome.hpp"
#include "SkyCycle.hpp"
#include <Application.hpp>
#include <ResourceManager.hpp>
#include <Camera.hpp>
#include <Interpolations.hpp>
#include <Math.hpp>

#include "ResourceDefinitions.hpp"

SunLight::SunLight(void) :
	m_vertices(new sf::Vertex[24]),
	m_count(24),
	m_used(0u),
	m_canCreateRain(false),
	m_timerRain(sf::Time::Zero),
	m_timerRainMax(sf::seconds(2.f)),
	m_animator(5.f, 0.f, 3.f, 0.4f),
	m_animation(1.f),
	m_cycle(nullptr)
{
	m_builder = octo::VertexBuilder(m_vertices.get(), m_count);
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

void SunLight::createSunLight(sf::Vector2f const & cameraSize, sf::Vector2f const & origin, sf::Color const & colorDay, sf::Color const & colorSunset, sf::Color const & colorNight, octo::VertexBuilder& builder)
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

	createBicolorQuad(dayUpLeft, dayUpRight, dayDownRight, dayDownLeft, colorDay, colorDay, builder);
	createBicolorQuad(sunsetLeft, sunsetRight, dayUpRight, dayUpLeft, colorSunset, colorDay, builder);
	createBicolorQuad(nightDownLeft, nightDownRight, sunsetRight, sunsetLeft, colorNight, colorSunset, builder);
	createBicolorQuad(nightUpLeft, nightUpRight, nightDownRight, nightDownLeft, colorNight, colorNight, builder);
}

void SunLight::setupLights(void)
{
	m_lightMapTexture.create(m_cameraSize.x * 1.2f, m_cameraSize.y * 1.2f);
	m_lightMap.setTexture(m_lightMapTexture.getTexture());
	m_lightTexture = octo::Application::getResourceManager().getTexture(LIGHT_PNG);
	//TODO: Check with and without
	m_lightTexture.setSmooth(true);
	m_lightSprite.setTexture(m_lightTexture);
	m_lightSprite.setTextureRect(sf::IntRect(0, 0, 512, 512));
	m_lightSprite.setOrigin(256.f, 256.f);

	//TODO: Know max light, implement it
	m_rotateLights.resize(18);
	
	for (std::size_t i = 0; i < m_rotateLights.size(); i++)
	{
		m_rotateLights[i].position.x += 100.f;
		float angle = i * 20 * octo::Deg2Rad;
		float cos = std::cos(angle);
		float sin = std::sin(angle);
		rotateVec(m_rotateLights[i].position, sin, cos);
		m_rotateLights[i].position += octo::Application::getCamera().getCenter() + sf::Vector2f(200.f, 400.f);
	}
	//m_rotateLights.push_back(Light(octo::Application::getCamera().getCenter() + sf::Vector2f(0.f, m_cameraSize.y / 4.f), sf::Vector2f(0.9f, 0.9f),  sf::Color(255, 255, 200, 150)));
}

void SunLight::setup(ABiome& biome, SkyCycle & skyCycle)
{
	m_cycle = &skyCycle;
	octo::Camera & camera = octo::Application::getCamera();
	m_cameraSize = camera.getSize();
	m_dayPos = sf::Vector2f(0.f, m_cameraSize.y);
	m_nightPos = sf::Vector2f(0.f, -m_cameraSize.y * 4.f);
	m_sunsetPos = sf::Vector2f(0.f, -m_cameraSize.y * 2.f);
	m_colorNight = biome.getNightLightColor();
	m_colorSunset = biome.getSunsetLightColor();
	m_colorDay = sf::Color::Transparent;
	m_colorDayRaining = sf::Color(100, 100, 100, 100);
	m_canCreateRain = biome.canCreateRain();
	m_animator.setup();
	setupLights();
}

void SunLight::computeDayColorValue(sf::Time frameTime)
{
	float weather = m_cycle->getWeatherValue();
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

void SunLight::updateLights(void)
{
	sf::FloatRect cameraRect = octo::Application::getCamera().getRectangle();
	sf::Vector2f position(cameraRect.left - m_cameraSize.x * 0.1f, cameraRect.top - m_cameraSize.y * 0.1f);

	m_lightMapTexture.clear(sf::Color::White);
	m_lightMap.setTexture(m_lightMapTexture.getTexture());
	m_lightMapTexture.draw(m_vertices.get(), m_used, sf::Triangles);
	for (std::size_t i = 0; i < m_rotateLights.size(); ++i)
	{
		m_lightSprite.setScale(m_rotateLights[i].scale * m_animation);
		m_lightSprite.setColor(m_rotateLights[i].color);
		m_lightSprite.setPosition(m_rotateLights[i].position);
		m_lightMapTexture.draw(m_lightSprite, sf::BlendAdd);
	}
	m_lightMapTexture.display();
	m_lightMap.setTextureRect(sf::IntRect(0.f, 0.f, m_cameraSize.x * 1.2f, m_cameraSize.y * 1.2f));
	//m_rotateLights[0].position = position;
	m_lightMap.setPosition(position);
}

void SunLight::update(sf::Time frameTime)
{
	m_builder.clear();
	m_animator.update(frameTime);
	m_animation = m_animator.getAnimation();

	sf::Vector2f position(m_cameraSize.x * 0.6, m_cameraSize.y * 0.6);
	if (m_canCreateRain)
		computeDayColorValue(frameTime);
	float dayValue = m_cycle->getDayValue();
	float nightValue = m_cycle->getNightValue();
	if (m_cycle->isDay())
		position -= octo::linearInterpolation(m_sunsetPos, m_dayPos, dayValue);
	else if (m_cycle->isNight())
		position -= octo::linearInterpolation(m_sunsetPos, m_nightPos, nightValue);

	float thunderValue = m_cycle->getThunderValue();
	if (thunderValue == 0.f)
		createSunLight(m_cameraSize, position, m_colorDay, m_colorSunset, m_colorNight, m_builder);
	else
	{
		sf::Color colorDay = octo::linearInterpolation(m_colorDay, sf::Color::Transparent, thunderValue);
		sf::Color colorSunset = octo::linearInterpolation(m_colorSunset, sf::Color::Transparent, thunderValue);
		sf::Color colorNight = octo::linearInterpolation(m_colorNight, sf::Color::Transparent, thunderValue);
		createSunLight(m_cameraSize, position, colorDay, colorSunset, colorNight, m_builder);
	}

	m_used = m_builder.getUsed();
	updateLights();
}

void SunLight::draw(sf::RenderTarget& render, sf::RenderStates states) const
{
	states.blendMode = sf::BlendMultiply;
	render.draw(m_lightMap, states);
}

void SunLight::rotateVec(sf::Vector2f & vector, float const cosAngle, float const sinAngle)
{
	float x = vector.x * cosAngle - vector.y * sinAngle;
	vector.y = vector.y * cosAngle + vector.x * sinAngle;
	vector.x = x;
}

void SunLight::rotateVec(sf::Vector2f & vector, sf::Vector2f const & origin, float const cosAngle, float const sinAngle)
{
	vector -= origin;
	rotateVec(vector, cosAngle, sinAngle);
	vector += origin;
}
