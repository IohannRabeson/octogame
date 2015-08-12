#include "Portal.hpp"
#include <Application.hpp>
#include <ResourceManager.hpp>
#include <PostEffectManager.hpp>

Portal::Portal(void)
{
	octo::ResourceManager& resources = octo::Application::getResourceManager();
	octo::PostEffectManager& postEffect = octo::Application::getPostEffectManager();

	m_shader.loadFromMemory(resources.getText(VORTEX_FRAG, sf::Shader::Fragment));
}

void Portal::update(sf::Time )
{

}

void Portal::draw(sf::RenderTarget &) const
{
}
