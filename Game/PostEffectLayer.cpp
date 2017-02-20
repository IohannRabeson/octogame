#include "PostEffectLayer.hpp"
#include <Application.hpp>
#include <ResourceManager.hpp>
#include <PostEffectManager.hpp>

std::unique_ptr<PostEffectLayer> PostEffectLayer::m_instance = nullptr;

PostEffectLayer::PostEffectLayer(void) { }

PostEffectLayer::~PostEffectLayer(void)
{
	clear();
}

PostEffectLayer & PostEffectLayer::getInstance(void)
{
	if (m_instance == nullptr)
	{
		m_instance.reset(new PostEffectLayer());
	}
	return *m_instance;
}

void PostEffectLayer::registerShader(std::string const & name, ResourceKey const & key)
{
	octo::ResourceManager & resources = octo::Application::getResourceManager();
	octo::PostEffectManager & postEffect = octo::Application::getPostEffectManager();

	octo::PostEffect postEffectShader;
	sf::Shader * shader = new sf::Shader();
	shader->loadFromMemory(resources.getText(key), sf::Shader::Fragment);
	postEffectShader.resetShader(shader);
	std::size_t shaderIndex = postEffect.addEffect(std::move(postEffectShader));
	m_shaders.emplace(name, std::make_pair(shaderIndex, shader));
}

void PostEffectLayer::enableShader(std::string const & name, bool enable)
{
	octo::Application::getPostEffectManager().enableEffect(getShaderIndex(name), enable);
}

void PostEffectLayer::clear(void)
{
	for (auto & it : m_shaders)
		delete it.second.second;
	m_shaders.clear();
}

sf::Shader & PostEffectLayer::getShader(std::string const & name)
{
	return (*m_shaders[name].second);
}

std::size_t PostEffectLayer::getShaderIndex(std::string const & name)
{
	auto it = m_shaders.find(name);
	if (it != m_shaders.end())
		return (it->second.first);
	return (0u);
}
