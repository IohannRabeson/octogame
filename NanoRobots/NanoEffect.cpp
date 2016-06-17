#include "NanoEffect.hpp"
#include "PostEffectLayer.hpp"
#include "Progress.hpp"
#include <Application.hpp>
#include <AudioManager.hpp>
#include <ResourceManager.hpp>
#include <GraphicsManager.hpp>
#include <PostEffectManager.hpp>
#include <Camera.hpp>

NanoEffect::NanoEffect(void) :
	m_vertices(new sf::Vertex[100]),
	m_count(100),
	m_used(0u),
	m_size(150.f, 150.f),
	m_color(255, 255, 255, 200),
	m_state(State::Active),
	m_isTransferHappen(false),
	m_glowingTimerMax(sf::seconds(2.f)),
	m_transferTimerMax(sf::seconds(4.f)),
	m_soundPlayed(false),
	m_lastNanoCount(0u),
	m_shader(PostEffectLayer::getInstance().getShader(CIRCLE_RAINBOW_FRAG)),
	m_shaderIndex(PostEffectLayer::getInstance().getShaderIndex(CIRCLE_RAINBOW_FRAG)),
	m_particle(new MusicSystem()),
	m_isTravelling(true),
	m_effectEnable(true)
{
	m_generator.setSeed("random");
	m_randomTimerMax = sf::seconds(m_generator.randomFloat(20.f, 50.f));
	m_builder = octo::VertexBuilder(m_vertices.get(), m_count);
	m_lastNanoCount = Progress::getInstance().getNanoRobotCount();

	octo::Application::getPostEffectManager().enableEffect(m_shaderIndex, false);
	m_shader.setParameter("fade_out_size", 100.f);
	m_shader.setParameter("alpha", 0.f);
}

void NanoEffect::playSound(void)
{
	if (m_soundPlayed == false)
	{
		std::size_t soundChoose = m_generator.randomInt(0u, 3u);
		octo::AudioManager& audio = octo::Application::getAudioManager();
		octo::ResourceManager& resource = octo::Application::getResourceManager();
	
		if (m_lastNanoCount != Progress::getInstance().getNanoRobotCount())
			audio.playSound(resource.getSound(OCTO_QUESTION_OGG), 0.7f, 1.f);
		switch (soundChoose)
		{
			case 0u:
				audio.playSound(resource.getSound(NANO_1_OGG), 0.3f, 1.f);
				break;
			case 1u:
				audio.playSound(resource.getSound(NANO_2_OGG), 0.3f, 1.f);
				break;
			case 2u:
				audio.playSound(resource.getSound(NANO_3_OGG), 0.3f, 1.f);
				break;
			default:
				break;
		}
		m_soundPlayed = true;
		m_particle->canEmit(true);
		m_particleTimer = sf::seconds(8.f);
	}
}

void NanoEffect::createLosange(sf::Vector2f const & size, sf::Vector2f const & origin, sf::Color const & color, octo::VertexBuilder& builder)
{
	sf::Vector2f up(0.f, -size.y);
	sf::Vector2f down(0.f, size.y);
	sf::Vector2f left(-size.x, 0.f);
	sf::Vector2f right(size.x, 0.f);

	up += origin;
	down += origin;
	left += origin;
	right += origin;

	builder.createTriangle(left, up, right, color);
	builder.createTriangle(left, down, right, color);
}

void NanoEffect::createEffect(sf::Vector2f const & size, sf::Vector2f const & origin, float glowingCoef, sf::Color color, octo::VertexBuilder& builder)
{
	color.a = color.a * (1 - glowingCoef);
	createLosange(size * glowingCoef, origin, color, builder);
}

void NanoEffect::update(sf::Time frameTime)
{
	m_builder.clear();

	m_particleTimer -= frameTime;
	m_glowingTimer += frameTime;
	switch (m_state)
	{
		case State::Active:
		{
			octo::PostEffectManager& postEffect = octo::Application::getPostEffectManager();
			if (!m_isTravelling && m_effectEnable)
				postEffect.enableEffect(m_shaderIndex, true);
			else
				postEffect.enableEffect(m_shaderIndex, false);
			sf::FloatRect const & screen = octo::Application::getCamera().getRectangle();
			float zoomFactor = octo::Application::getGraphicsManager().getVideoMode().height / screen.height;
			m_shader.setParameter("position", (m_position.x - screen.left) * zoomFactor, octo::Application::getGraphicsManager().getVideoMode().height + (-m_position.y + screen.top) * zoomFactor);
			m_shader.setParameter("time", 0.5f * m_glowingTimer.asSeconds());
			m_shader.setParameter("radius", 120.f);
			m_shader.setParameter("alpha", 1.f);
			m_shader.setParameter("color_size", 0.005f);
			break;
		}
		case State::Transfer:
		{
			playSound();
			m_transferTimer += frameTime;
			if (m_effectEnable)
			{
				float transition = std::min(m_transferTimer / m_transferTimerMax, 1.f);
				sf::FloatRect const & screen = octo::Application::getCamera().getRectangle();
				float zoomFactor = octo::Application::getGraphicsManager().getVideoMode().height / screen.height;
				m_shader.setParameter("position", (m_position.x - screen.left) * zoomFactor, octo::Application::getGraphicsManager().getVideoMode().height + (-m_position.y + screen.top) * zoomFactor);
				m_shader.setParameter("time", 0.5f * m_glowingTimer.asSeconds());
				m_shader.setParameter("radius", octo::linearInterpolation(120.f, 1500.f, transition));
				m_shader.setParameter("color_size", octo::linearInterpolation(0.005f, 0.001f, transition));
			}
			if (m_transferTimer >= m_transferTimerMax)
			{
				m_isTransferHappen = true;
				m_transferTimer = sf::Time::Zero;
				m_randomTimer = sf::Time::Zero;
				m_state = State::FadeOut;
			}
			break;
		}
		case State::FadeOut:
		{
			m_transferTimer += frameTime;
			sf::FloatRect const & screen = octo::Application::getCamera().getRectangle();
			float zoomFactor = octo::Application::getGraphicsManager().getVideoMode().height / screen.height;
			m_shader.setParameter("position", (m_position.x - screen.left) * zoomFactor, octo::Application::getGraphicsManager().getVideoMode().height + (-m_position.y + screen.top) * zoomFactor);
			m_shader.setParameter("time", 0.5f * m_glowingTimer.asSeconds());
			m_shader.setParameter("alpha", 1.f - std::min(1.f, m_transferTimer / m_transferTimerMax));
			if (m_transferTimer >= m_transferTimerMax)
			{
				m_transferTimer = sf::Time::Zero;
				m_state = State::Wait;
				octo::PostEffectManager& postEffect = octo::Application::getPostEffectManager();
				postEffect.enableEffect(m_shaderIndex, false);
			}
			break;
		}
		case State::Random:
		{
			playSound();
			m_glowingTimer += frameTime;
			if (m_glowingTimer >= m_glowingTimerMax)
			{
				m_glowingTimer = sf::Time::Zero;
				m_state = State::Wait;
			}
			createEffect(m_size, m_position, m_glowingTimer / m_glowingTimerMax, m_color, m_builder);
			break;
		}
		case State::Wait:
		{
			m_randomTimer += frameTime;
			if (m_randomTimer >= m_randomTimerMax)
			{
				m_randomTimer = sf::Time::Zero;
				m_glowingTimer = sf::Time::Zero;
				m_state = State::Random;
			}
			if (m_particleTimer <= sf::Time::Zero)
			{
				m_soundPlayed = false;
				m_particle->canEmit(false);
			}
			break;
		}
		default:
			break;
	}

	m_particle->update(frameTime);
	m_particle->setEmitter(m_position);
	Progress const & progress = Progress::getInstance();
	m_lastNanoCount = progress.getNanoRobotCount();
	m_used = m_builder.getUsed();
}

void NanoEffect::onTransfer(void)
{
	if (m_isTransferHappen == false)
	{
		m_state = State::Transfer;
		m_transferTimer = sf::Time::Zero;
	}
}

void NanoEffect::setState(State state)
{
	m_state = state;
}

void NanoEffect::setTravelling(bool travelling)
{
	m_isTravelling = travelling;
}

void NanoEffect::setEffectEnable(bool enable)
{
	m_effectEnable = enable;
}

bool NanoEffect::getEffectEnable(void) const
{
	return (m_effectEnable);
}

void NanoEffect::draw(sf::RenderTarget & render, sf::RenderStates states) const
{
	m_particle->draw(render);
	render.draw(m_vertices.get(), m_used, sf::Triangles, states);
}

sf::Vector2f const & NanoEffect::getPosition(void) const
{
	return m_position;
}

sf::Color const & NanoEffect::getColor(void) const
{
	return m_color;
}

sf::Vector2f const & NanoEffect::getNanoScale(void) const
{
	return m_nanoScale;
}

void NanoEffect::setPosition(sf::Vector2f const & position)
{
	m_position = position;
}

void NanoEffect::setColor(sf::Color const & color)
{
	m_color = color;
}

void NanoEffect::setNanoScale(sf::Vector2f const & scale)
{
	m_nanoScaleOrigin = scale;
	m_nanoScaleZoom = scale * 2.f;
	m_nanoScale = scale;
}
