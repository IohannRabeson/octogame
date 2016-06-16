#include "TVScreen.hpp"
#include <Application.hpp>
#include <GraphicsManager.hpp>
#include <ResourceManager.hpp>
#include <PostEffectManager.hpp>
#include <Camera.hpp>

TVScreen::TVScreen(void) :
	ANpc(TV_OSS)
{
	setSize(sf::Vector2f(25.f, 140.f));
	setOrigin(sf::Vector2f(90.f, 100.f));
	setScale(0.8f);
	setTextOffset(sf::Vector2f(-20.f, -10.f));
	setTimerMax(sf::seconds(8.0f));
	setup();

	setupBox(this, static_cast<std::size_t>(GameObjectType::Npc), static_cast<std::size_t>(GameObjectType::PlayerEvent));

	octo::ResourceManager & resources = octo::Application::getResourceManager();
	octo::PostEffectManager & postEffect = octo::Application::getPostEffectManager();

	m_shader.loadFromMemory(resources.getText(DUPLICATE_SCREEN_FRAG), sf::Shader::Fragment);
	octo::PostEffect postEffectShader;
	postEffectShader.resetShader(&m_shader);
	m_shaderIndex = postEffect.addEffect(std::move(postEffectShader));

	m_tvScreen.width = 480.f;
	m_tvScreen.height = 270.f;
	m_shader.setParameter("offset", 1.f / 300.f);
	sf::Transform kernel(
			-1.f, -1.f, -1.f,
			-1.f, 9.f, -1.f,
			-1.f, -1.f, -1.f
		);
	m_shader.setParameter("kernel", kernel);
}

void TVScreen::setup(void)
{
	typedef octo::CharacterAnimation::Frame			Frame;

	getIdleAnimation().setFrames({
			Frame(sf::seconds(0.4f), {0u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {1u, sf::FloatRect(), sf::Vector2f()}),
			});
	getIdleAnimation().setLoop(octo::LoopMode::Loop);

	getSpecial1Animation().setFrames({
			Frame(sf::seconds(0.4f), {0u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {1u, sf::FloatRect(), sf::Vector2f()}),
			});
	getSpecial1Animation().setLoop(octo::LoopMode::NoLoop);

	setupMachine();
}

void TVScreen::setupMachine(void)
{
	typedef octo::CharacterSprite::ACharacterState	State;
	typedef octo::FiniteStateMachine::StatePtr		StatePtr;

	octo::FiniteStateMachine	machine;
	StatePtr					idleState;
	StatePtr					special1State;

	idleState = std::make_shared<State>("0", getIdleAnimation(), getSprite());
	special1State = std::make_shared<State>("1", getSpecial1Animation(), getSprite());

	machine.setStart(idleState);
	machine.addTransition(Idle, idleState, idleState);
	machine.addTransition(Idle, special1State, idleState);

	machine.addTransition(Special1, idleState, special1State);

	setMachine(machine);
	setNextEvent(Idle);
}

void TVScreen::updateState(void)
{
	octo::PostEffectManager& postEffect = octo::Application::getPostEffectManager();
	m_tvScreen.left = ANpc::getPosition().x;
	m_tvScreen.top = ANpc::getPosition().y;

	sf::FloatRect const & screen = octo::Application::getCamera().getRectangle();
	if (screen.intersects(m_tvScreen))
	{
		postEffect.enableEffect(m_shaderIndex, true);
		float zoomFactor = octo::Application::getGraphicsManager().getVideoMode().height / screen.height;
		float width = octo::Application::getGraphicsManager().getVideoMode().width;
		float height = octo::Application::getGraphicsManager().getVideoMode().height;
		sf::Vector2f pos = sf::Vector2f(((m_tvScreen.left - screen.left) * zoomFactor) / width, (height + (-m_tvScreen.top + screen.top) * zoomFactor) / height);
		m_shader.setParameter("top_left", pos);
		sf::Vector2f size = sf::Vector2f(m_tvScreen.width * zoomFactor / width, m_tvScreen.height * zoomFactor / height);
		m_shader.setParameter("bot_right", pos.x + size.x, pos.y + size.y);
		m_shader.setParameter("size", size.x, size.y);
	}
	else
	{
		postEffect.enableEffect(m_shaderIndex, false);
	}

	octo::CharacterSprite & sprite = getSprite();

	if (sprite.getCurrentEvent() == Special1)
	{
		if (sprite.isTerminated())
		{
			sprite.setNextEvent(Idle);
			addTimer(-getTimer());
		}
	}
	else if (sprite.getCurrentEvent() == Idle)
	{
		if (getTimer() >= getTimerMax())
		{
			addTimer(-getTimerMax());
			sprite.setNextEvent(Special1);
		}
	}
}
