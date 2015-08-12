#include "Portal.hpp"
#include "ResourceDefinitions.hpp"
#include <Application.hpp>
#include <GraphicsManager.hpp>
#include <ResourceManager.hpp>
#include <PostEffectManager.hpp>
#include <Camera.hpp>

Portal::Portal(void) :
	m_position(200.f, 200.f),
	m_shaderIndex(0u),
	m_radius(50.f),
	m_timer(0.f),
	m_timerMax(3.5f),
	m_side(true)
{
	octo::ResourceManager& resources = octo::Application::getResourceManager();
	octo::PostEffectManager& postEffect = octo::Application::getPostEffectManager();

	m_shader.loadFromMemory(resources.getText(VORTEX_FRAG), sf::Shader::Fragment);
	m_shader.setParameter("radius", m_radius);
	m_shader.setParameter("center", m_position.x, m_position.y);
	m_shader.setParameter("time_max", m_timerMax);
	m_shaderIndex = postEffect.addShader(m_shader, true);

	PortalParticle::Prototype	prototype;

	static float const	Size = 8.f;
	prototype.emplace_back(sf::Vertex({-Size, Size}));
	prototype.emplace_back(sf::Vertex({Size, -Size}));
	prototype.emplace_back(sf::Vertex({-Size, -Size}));
	m_particles.reset(prototype, sf::Triangles, 1000);
	m_particles.setEmitter(m_position);
}

void Portal::update(sf::Time frametime)
{
	m_particles.update(frametime);
	octo::PostEffectManager& postEffect = octo::Application::getPostEffectManager();

	if (m_timer > m_timerMax)
		m_side = false;
	else if (m_timer < -m_timerMax)
		m_side = true;

	if (m_side)
		m_timer += frametime.asSeconds();
	else
		m_timer -= frametime.asSeconds();

	postEffect.enableShader(m_shaderIndex, false);
	sf::FloatRect const & screen = octo::Application::getCamera().getRectangle();
	if (m_position.x + m_radius > screen.left && m_position.x - m_radius < screen.left + screen.width)
	{
		if (m_position.y + m_radius > screen.top && m_position.y - m_radius < screen.top + screen.height)
		{
			postEffect.enableShader(m_shaderIndex, true);
			m_shader.setParameter("time", m_timer);
			m_shader.setParameter("resolution", octo::Application::getGraphicsManager().getVideoMode().width, octo::Application::getGraphicsManager().getVideoMode().height);
			m_shader.setParameter("center", m_position.x - screen.left, octo::Application::getGraphicsManager().getVideoMode().height - m_position.y + screen.top);
		}
	}
}

void Portal::draw(sf::RenderTarget & render) const
{
	m_particles.draw(render);
}
