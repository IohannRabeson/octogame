#ifndef POSTEFFECTLAYER_HPP
# define POSTEFFECTLAYER_HPP

# include <SFML/Graphics/Shader.hpp>
# include "ResourceDefinitions.hpp"
# include <memory>
# include <map>

class PostEffectLayer
{
public:
	PostEffectLayer(void);
	virtual ~PostEffectLayer(void);

	static PostEffectLayer & getInstance(void);

	sf::Shader & getShader(ResourceKey key);
	std::size_t getShaderIndex(ResourceKey key);
	void registerShader(ResourceKey key);
	void enableShader(ResourceKey key, bool enable);

	void clear(void);

private:
	static std::unique_ptr<PostEffectLayer>	m_instance;

	std::map<std::string, std::pair<std::size_t, sf::Shader*>>		m_shaders;

};

#endif
