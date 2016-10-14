#include "FinalPortal.hpp"
#include <Application.hpp>
#include <GraphicsManager.hpp>
#include <Camera.hpp>

FinalPortal::FinalPortal(Level destination, ResourceKey key, ResourceKey shader, sf::Color centerColor) :
	Portal(destination, key, shader, centerColor),
	m_forceActivate(false)
{ }

FinalPortal::~FinalPortal(void) { }

void FinalPortal::draw(sf::RenderTarget& render, sf::RenderStates) const
{
	if (m_forceActivate)
		m_particles.draw(render);
}

bool FinalPortal::zoom(void) const
{
	return false;
}

void FinalPortal::forceActivate(void)
{
	m_forceActivate = true;
}

void FinalPortal::update(sf::Time frametime)
{
	m_particles.update(frametime);

	if (m_forceActivate)
	{
		m_particles.setMaxParticle(m_timerActivate / m_timerActivateMax * static_cast<float>(m_maxParticle));
		m_timerActivate += frametime.asSeconds();
		if (m_timerActivate >= m_timerActivateMax)
		{
			m_isActive = true;
			m_timerActivate = m_timerActivateMax;
			m_particles.setMaxParticle(m_maxParticle);
		}
	}

	sf::FloatRect const & screen = octo::Application::getCamera().getRectangle();
	if (m_position.x + m_radius > screen.left && m_position.x - m_radius < screen.left + screen.width)
	{
		if (m_position.y + m_radius > screen.top && m_position.y - m_radius < screen.top + screen.height)
		{
			float zoomFactor = octo::Application::getGraphicsManager().getVideoMode().height / screen.height;
			PostEffectLayer::getInstance().enableShader(m_shaderName, true);
			m_shader.setParameter("time", m_timerActivate);
			m_shader.setParameter("radius", m_radius * zoomFactor);
			m_shader.setParameter("resolution", octo::Application::getGraphicsManager().getVideoMode().width, octo::Application::getGraphicsManager().getVideoMode().height);
			m_shader.setParameter("center", (m_position.x - screen.left) * zoomFactor, octo::Application::getGraphicsManager().getVideoMode().height + (-m_position.y + screen.top) * zoomFactor);
		}
	}

	updateSound();
}
