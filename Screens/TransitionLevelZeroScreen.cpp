#include "TransitionLevelZeroScreen.hpp"

#include "Progress.hpp"

#include <Application.hpp>
#include <StateManager.hpp>
#include <ResourceManager.hpp>
#include <AudioManager.hpp>
#include "ResourceDefinitions.hpp"
#include "TextManager.hpp"

#include <sstream>
#include <cwchar>

TransitionLevelZeroScreen::TransitionLevelZeroScreen() :
	m_bubbleCount(5u),
	m_bubble(new BubbleText[m_bubbleCount]),
	m_time(sf::Time::Zero),
	m_index(0u),
	m_startTimerMax(sf::seconds(1.f)),
	m_timerShaderRocketMax(sf::seconds(4.f)),
	m_soundPlayed1(false),
	m_soundPlayed2(false)
{
	octo::Application::getCamera().setCenter(0.f, 0.f);
	setupText();
	setupSprite();
	PostEffectLayer::getInstance().enableShader(CUTSCENE_FRAG, true);
	PostEffectLayer::getInstance().getShader(CUTSCENE_FRAG).setUniform("height", 0.15f);
	PostEffectLayer::getInstance().getShader(CUTSCENE_FRAG).setUniform("time", 1.f);

	PostEffectLayer::getInstance().enableShader(CIRCLE_RAINBOW_FRAG, true);
	PostEffectLayer::getInstance().getShader(CIRCLE_RAINBOW_FRAG).setUniform("alpha", 1.f);
	PostEffectLayer::getInstance().getShader(CIRCLE_RAINBOW_FRAG).setUniform("fade_out_size", 100.f);
	PostEffectLayer::getInstance().getShader(CIRCLE_RAINBOW_FRAG).setUniform("position", octo::Application::getCamera().getSize() / 2.f);
	PostEffectLayer::getInstance().getShader(CIRCLE_RAINBOW_FRAG).setUniform("radius", 1500.f);
	PostEffectLayer::getInstance().getShader(CIRCLE_RAINBOW_FRAG).setUniform("color_size", 0.001f);
}

void	TransitionLevelZeroScreen::setupText()
{
	std::string							keyName;
	octo::Camera const &				camera = octo::Application::getCamera();
	TextManager &						textManager = TextManager::getInstance();

	if (Progress::getInstance().getNextDestination() != Level::Random)
		keyName = "transitionLevelZero";
	else
		keyName = "transitionLevelZeroToRandom";

	std::vector<std::wstring> const &	texts = textManager.getTexts(keyName);

	for (std::size_t i = 0u; i < texts.size(); i++)
	{
		m_bubble[i].setup(texts[i], sf::Color::White, 28u, 700.f);
		m_bubble[i].setPosition(camera.getCenter());
		m_bubble[i].setType(ABubble::Type::None);
		m_bubble[i].setActive(true);
	}

	m_timerMax.resize(m_bubbleCount);
	m_timerMax[0] = sf::seconds(5.5f);
	m_timerMax[1] = sf::seconds(6.f);
	m_timerMax[2] = sf::seconds(7.f);
	m_timerMax[3] = sf::seconds(8.f);
	m_timerMax[4] = sf::seconds(9.f);
}

void	TransitionLevelZeroScreen::setupSprite()
{
	typedef octo::SpriteAnimation::Frame	Frame;
	octo::Camera const &					camera = octo::Application::getCamera();
	octo::ResourceManager &					resources = octo::Application::getResourceManager();

	m_sprite.setSpriteSheet(resources.getSpriteSheet(NANO_GROUND_TRANSFORM_OSS));
	m_sprite.setScale(1.f, 1.f);

	m_animation.setFrames({	Frame(sf::seconds(0.2f), 0u),
							Frame(sf::seconds(0.2f), 1u),
							Frame(sf::seconds(0.2f), 2u),
							Frame(sf::seconds(0.2f), 3u),
							Frame(sf::seconds(0.2f), 2u),
							Frame(sf::seconds(0.2f), 1u)});
	m_animation.setLoop(octo::LoopMode::Loop);

	m_sprite.setAnimation(m_animation);
	m_sprite.play();
	m_sprite.setPosition(camera.getCenter() + sf::Vector2f(-40.f, -4.f));
}

void	TransitionLevelZeroScreen::start()
{
	InputListener::addInputListener();
}

void	TransitionLevelZeroScreen::pause()
{
}

void	TransitionLevelZeroScreen::resume()
{
}

void	TransitionLevelZeroScreen::stop()
{
	if (Progress::getInstance().getNextDestination() != Level::Random)
	{
		Progress::getInstance().setFirstTime(false);
		Progress::getInstance().save();
	}
	InputListener::removeInputListener();
}

void	TransitionLevelZeroScreen::playSound(std::size_t index)
{
	if (m_soundPlayed1 == false && index == 0u)
	{
		octo::Application::getAudioManager().playSound(octo::Application::getResourceManager().getSound(OBJECT_NANO_2_OGG), 0.5f);
		m_soundPlayed1 = true;
	}
	if (m_soundPlayed2 == false && index == 2u)
	{
		octo::Application::getAudioManager().playSound(octo::Application::getResourceManager().getSound(OBJECT_NANO_3_OGG), 0.5f);
		m_soundPlayed2 = true;
	}
}

void	TransitionLevelZeroScreen::update(sf::Time frameTime)
{
	if (m_index >= m_bubbleCount)
	{
		octo::StateManager & states = octo::Application::getStateManager();
		states.change("game");
		return;
	}
	if (m_startTimer <= m_startTimerMax)
	{
		m_startTimer += frameTime;
		return;
	}
	playSound(m_index);
	m_time += frameTime;
	for (std::size_t i = 0; i < m_bubbleCount; i++)
		m_bubble[i].setType(ABubble::Type::None);

	m_bubble[m_index].setType(ABubble::Type::Speak);
	m_bubble[m_index].update(frameTime);
	if (m_time > m_timerMax[m_index])
	{
		m_time = sf::Time::Zero;
		m_index++;
	}
	m_timeCircleRainbow += frameTime;
	m_timerShaderRocket += frameTime;
	PostEffectLayer::getInstance().getShader(CIRCLE_RAINBOW_FRAG).setUniform("time", 0.5f * m_timeCircleRainbow.asSeconds());
	PostEffectLayer::getInstance().getShader(ROCKET_TAKEOFF_FRAG).setUniform("intensity", octo::linearInterpolation(0.2f, 0.f, std::min(m_timerShaderRocket / m_timerShaderRocketMax, 1.f)));
	PostEffectLayer::getInstance().getShader(ROCKET_TAKEOFF_FRAG).setUniform("time", m_timeCircleRainbow.asSeconds());

	m_sprite.update(frameTime);
}

bool TransitionLevelZeroScreen::onInputPressed(InputListener::OctoKeys const & event)
{
	switch (event)
	{
		case OctoKeys::Entrance:
		case OctoKeys::Capacity:
		case OctoKeys::Jump:
		case OctoKeys::Elevator:
		case OctoKeys::SelectMenu:
			m_index++;
			m_time = sf::Time::Zero;
			break;
		default:
			break;
	}
	return (true);
}

void	TransitionLevelZeroScreen::draw(sf::RenderTarget& render)const
{
	if (m_index >= m_bubbleCount)
		return;
	render.clear(sf::Color(55, 0, 0));

	if (m_startTimer >= m_startTimerMax)
		render.draw(m_sprite);
	if (m_index < m_bubbleCount)
		m_bubble[m_index].draw(render);
}
