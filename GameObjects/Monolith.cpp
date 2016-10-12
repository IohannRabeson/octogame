#include "Monolith.hpp"
#include "PhysicsEngine.hpp"
#include "CircleShape.hpp"
#include "MonolithStep.hpp"
#include "Progress.hpp"
#include "CharacterOcto.hpp"
#include <Application.hpp>
#include <ResourceManager.hpp>
#include <GraphicsManager.hpp>
#include <Camera.hpp>

Monolith::Monolith(sf::Vector2f const & scale, sf::Vector2f const & position) :
	InstanceDecor(MONOLITH_PEDESTAL_OSS, scale, position, 1u),
	m_vertices(new sf::Vertex[100u]),
	m_size(500.f, 500.f),
	m_box(PhysicsEngine::getShapeBuilder().createCircle(false)),
	m_timerMax(sf::seconds(2.f)),
	m_explosionShaderTimerMax(sf::seconds(1.f)),
	m_used(0u),
	m_state(None),
	m_offset(0.f),
	m_transitionStartEndPosition(0.f)
{
	PostEffectLayer::getInstance().getShader(CIRCLE_WAVE_FRAG).setParameter("center", 0.5f, 0.5f);
	PostEffectLayer::getInstance().getShader(CIRCLE_WAVE_FRAG).setParameter("radius", 0.f);
	PostEffectLayer::getInstance().getShader(CIRCLE_WAVE_FRAG).setParameter("fade_out_size", 400.f);
	PostEffectLayer::getInstance().getShader(CIRCLE_WAVE_FRAG).setParameter("fade_in_size", 100.f);
	PostEffectLayer::getInstance().getShader(CIRCLE_WAVE_FRAG).setParameter("deformation_factor", 0.03f);
	PostEffectLayer::getInstance().enableShader(CIRCLE_WAVE_FRAG, true);

	octo::ResourceManager & resources = octo::Application::getResourceManager();

	m_animationMonolith.resize(6);
	m_spriteMonolith.resize(6);
	m_position.resize(6);
	m_endPosition.resize(6);

	octo::SpriteAnimation::FrameList	frames;
	frames.emplace_back(sf::seconds(0.1f), 0u);

	m_center = position + sf::Vector2f(300, 337);
	// Down left 2
	m_position[0] = position + sf::Vector2f(143.f, 237.f);
	m_endPosition[0] = m_position[0] + sf::Vector2f(-150.f, 150.f);
	m_spriteMonolith[0].setSpriteSheet(resources.getSpriteSheet(MONOLITH_DOWN_LEFT_2_OSS));
	// Up
	m_position[1] = position + sf::Vector2f(168.f, 112.f);
	m_endPosition[1] = m_position[1] + sf::Vector2f(0.f, -150.f);
	m_spriteMonolith[1].setSpriteSheet(resources.getSpriteSheet(MONOLITH_UP_OSS));
	// Down left 1
	m_position[2] = position + sf::Vector2f(131.f, 193.f);
	m_endPosition[2] = m_position[2] + sf::Vector2f(-150.f, 150.f);
	m_spriteMonolith[2].setSpriteSheet(resources.getSpriteSheet(MONOLITH_DOWN_LEFT_1_OSS));
	// Down right
	m_position[3] = position + sf::Vector2f(250.f, 218.f);
	m_endPosition[3] = m_position[3] + sf::Vector2f(150.f, 150.f);
	m_spriteMonolith[3].setSpriteSheet(resources.getSpriteSheet(MONOLITH_DOWN_RIGHT_OSS));
	// Gemme
	m_position[4] = position + sf::Vector2f(200.f, 237.f);
	m_endPosition[4] = m_position[4] + sf::Vector2f(0.f, 0.f);
	m_spriteMonolith[4].setSpriteSheet(resources.getSpriteSheet(MONOLITH_CENTER_OSS));
	// Fissure white
	m_position[5] = position + sf::Vector2f(168.f, 193.f);
	m_endPosition[5] = m_position[5] + sf::Vector2f(0.f, 0.f);
	m_spriteMonolith[5].setSpriteSheet(resources.getSpriteSheet(MONOLITH_FISSURE_WHITE_OSS));

	for (std::size_t i = 0u; i < m_animationMonolith.size(); i++)
	{
		m_animationMonolith[i].setFrames(frames);
		m_animationMonolith[i].setLoop(octo::LoopMode::NoLoop);

		m_spriteMonolith[i].setScale(scale);
		m_spriteMonolith[i].setPosition(m_position[i]);
		m_spriteMonolith[i].setAnimation(m_animationMonolith[i]);
		m_spriteMonolith[i].play();
	}

	m_steps.emplace_back(new MonolithStep(MONOLITH_STEP_1_OSS, position + sf::Vector2f(6.f, 262.f), scale));
	m_steps.emplace_back(new MonolithStep(MONOLITH_STEP_2_OSS, position + sf::Vector2f(56.f, 194.f), scale));
	m_steps.emplace_back(new MonolithStep(MONOLITH_STEP_3_OSS, position + sf::Vector2f(50.f, 106.f), scale));
	m_steps.emplace_back(new MonolithStep(MONOLITH_STEP_4_OSS, position + sf::Vector2f(112.f, 44.f), scale));
	m_steps.emplace_back(new MonolithStep(MONOLITH_STEP_5_OSS, position + sf::Vector2f(225.f, 12.f), scale));
	m_steps.emplace_back(new MonolithStep(MONOLITH_STEP_6_OSS, position + sf::Vector2f(268.f, 31.f), scale));
	m_steps.emplace_back(new MonolithStep(MONOLITH_STEP_7_OSS, position + sf::Vector2f(369.f, 62.f), scale));
	m_steps.emplace_back(new MonolithStep(MONOLITH_STEP_8_OSS, position + sf::Vector2f(331.f, 106.f), scale));
	m_steps.emplace_back(new MonolithStep(MONOLITH_STEP_9_OSS, position + sf::Vector2f(356.f, 162.f), scale));
	m_steps.emplace_back(new MonolithStep(MONOLITH_STEP_10_OSS, position + sf::Vector2f(419.f, 206.f), scale));
	m_steps.emplace_back(new MonolithStep(MONOLITH_STEP_11_OSS, position + sf::Vector2f(363.f, 256.f), scale));
	m_steps.emplace_back(new MonolithStep(MONOLITH_STEP_12_OSS, position + sf::Vector2f(412.f, 275.f), scale));
	m_steps.emplace_back(new MonolithStep(MONOLITH_STEP_13_OSS, position + sf::Vector2f(387.f, 375.f), scale));
	m_steps.emplace_back(new MonolithStep(MONOLITH_STEP_14_OSS, position + sf::Vector2f(287.f, 425.f), scale));
	m_steps.emplace_back(new MonolithStep(MONOLITH_STEP_15_OSS, position + sf::Vector2f(237.f, 462.f), scale));
	m_steps.emplace_back(new MonolithStep(MONOLITH_STEP_16_OSS, position + sf::Vector2f(194.f, 431.f), scale));
	m_steps.emplace_back(new MonolithStep(MONOLITH_STEP_17_OSS, position + sf::Vector2f(119.f, 437.f), scale));
	m_steps.emplace_back(new MonolithStep(MONOLITH_STEP_18_OSS, position + sf::Vector2f(31.f, 369.f), scale));

	for (std::size_t i = 0; i < Progress::getInstance().getActivatedMonolith(); i++)
		m_steps[i]->activate();

	m_box->setGameObject(this);
	m_box->setCollisionType(static_cast<std::size_t>(GameObjectType::Monolith));
	m_box->setCollisionMask(static_cast<std::size_t>(GameObjectType::Player));
	m_box->setRadius(500.f);
	m_box->setApplyGravity(false);
	m_box->setType(AShape::Type::e_trigger);
	m_box->setPosition(getSprite().getPosition() + getSprite().getGlobalSize() / 2.f - sf::Vector2f(m_box->getRadius(), m_box->getRadius()));
	m_box->update();

	m_builder = octo::VertexBuilder(m_vertices.get(), 100u);
}

void Monolith::addMapOffset(float x, float y)
{
	InstanceDecor::addMapOffset(x, y);
	for (auto & sprite : m_spriteMonolith)
		sprite.setPosition(sprite.getPosition().x + x, sprite.getPosition().y + y);
	for (auto & step : m_steps)
		step->addMapOffset(x, y);
	m_box->setPosition(m_box->getPosition() + sf::Vector2f(x, y));
	m_box->update();
}

void Monolith::setPosition(sf::Vector2f const & position)
{
	Monolith::setPosition(position);
	for (auto & sprite : m_spriteMonolith)
		sprite.setPosition(position);
	for (auto & step : m_steps)
		step->setPosition(position);
	m_box->setPosition(m_spriteMonolith[0].getPosition() + m_spriteMonolith[0].getGlobalSize() / 2.f - sf::Vector2f(m_box->getRadius(), m_box->getRadius()));
	m_box->update();
}

void Monolith::collideOcto(CharacterOcto * octo)
{
	if (m_state == None)
	{
		if (Progress::getInstance().getActivatedMonolith() < Progress::getInstance().countRandomDiscover())
		{
			octo->enableCutscene(true, true);
			m_state = StartEffect;
		}
		else if (Progress::getInstance().getActivatedMonolith() == 18)
		{
			m_state = StartFinalScene;
			m_size = sf::Vector2f(1000.f, 1000.f);
			std::cout << "end" << std::endl;
		}
	}
}

void Monolith::update(sf::Time frameTime)
{
	InstanceDecor::update(frameTime);

	//m_explosionShaderTimer += frameTime;
	//if (m_explosionShaderTimer >= m_explosionShaderTimerMax)
	//	m_explosionShaderTimer = sf::Time::Zero;

	//sf::FloatRect const & screen = octo::Application::getCamera().getRectangle();
	//float zoomFactor = octo::Application::getGraphicsManager().getVideoMode().height / screen.height;
	//PostEffectLayer::getInstance().getShader(CIRCLE_WAVE_FRAG).setParameter("center", (m_center.x - screen.left) * zoomFactor, octo::Application::getGraphicsManager().getVideoMode().height + (-m_center.y + screen.top) * zoomFactor);
	//PostEffectLayer::getInstance().getShader(CIRCLE_WAVE_FRAG).setParameter("radius", m_explosionShaderTimer / m_explosionShaderTimerMax * 2000.0f);

	m_offset += frameTime.asSeconds();
	switch (m_state)
	{
		case StartEffect:
		{
			m_timer += frameTime;
			if (m_timer >= m_timerMax)
			{
				m_timer = sf::Time::Zero;
				m_state = Activate;
			}
			break;
		}
		case Activate:
		{
			Progress & progress = Progress::getInstance();

			for (std::size_t i = progress.getActivatedMonolith(); i < progress.countRandomDiscover(); i++)
				m_steps[i]->firstActivate();
			progress.setActivatedMonolith(progress.countRandomDiscover());
			m_state = None;
			break;
		}
		case StartFinalScene:
		{
			m_timer += frameTime;
			for (std::size_t i = 0u; i < m_spriteMonolith.size(); i++)
			{
				m_position[i] += sf::Vector2f(0.f, -100.f) * frameTime.asSeconds();
				m_endPosition[i] += sf::Vector2f(0.f, -100.f) * frameTime.asSeconds();
			}
			if (m_timer > m_timerMax)
			{
				m_timer = sf::Time::Zero;
				m_state = FinalLosange;
			}
			break;
		}
		case FinalLosange:
		{
			m_timer += frameTime;
			m_builder.clear();
			sf::Vector2f pos = m_center;
			createEffect(m_size, pos, std::pow(std::min(1.f, m_timer / m_timerMax), 0.567f), sf::Color(240, 25, 25, 200), m_builder);
			m_used = m_builder.getUsed();
			if (m_timer > m_timerMax)
			{
				m_timer = sf::Time::Zero;
				m_state = FinalExplosion;
			}
			break;
		}
		case FinalExplosion:
		{
			m_timer += frameTime;
			m_transitionStartEndPosition = std::min(std::pow(m_timer / m_timerMax, 0.22f), 1.f);

			if (m_timer > m_timerMax)
			{
				m_timer = sf::Time::Zero;
				m_state = PortalAppear;
			}
			break;
		}
		default:
			break;
	}
	for (auto & step : m_steps)
		step->update(frameTime);
	for (std::size_t i = 0u; i < m_spriteMonolith.size(); i++)
	{
		sf::Vector2f pos = octo::linearInterpolation(m_position[i], m_endPosition[i], m_transitionStartEndPosition) + sf::Vector2f(0.f, std::cos(m_offset) * 5.f);
		m_spriteMonolith[i].setPosition(pos);
		m_spriteMonolith[i].update(frameTime);
	}
}

void Monolith::draw(sf::RenderTarget& render, sf::RenderStates states) const
{
	InstanceDecor::draw(render, states);
	for (auto & sprite : m_spriteMonolith)
		sprite.draw(render, states);
	for (auto & step : m_steps)
		step->draw(render, states);
	render.draw(m_vertices.get(), m_used, sf::Triangles, states);
}

void Monolith::drawFront(sf::RenderTarget& render, sf::RenderStates states) const
{
	InstanceDecor::drawFront(render, states);
}

void Monolith::createLosange(sf::Vector2f const & size, sf::Vector2f const & origin, sf::Color const & color, octo::VertexBuilder& builder)
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

void Monolith::createEffect(sf::Vector2f const & size, sf::Vector2f const & origin, float glowingCoef, sf::Color color, octo::VertexBuilder& builder)
{
	color.a = color.a * (1 - glowingCoef);
	createLosange(size * glowingCoef, origin, color, builder);
}
