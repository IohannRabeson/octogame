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

	sf::Shader & getShader(std::string const & name);
	std::size_t getShaderIndex(std::string const & name);
	void registerShader(std::string const & name, ResourceKey const & key);
	void enableShader(std::string const & name, bool enable);

	void clear(void);

private:
	static std::unique_ptr<PostEffectLayer>	m_instance;

	std::map<std::string, std::pair<std::size_t, sf::Shader*>>		m_shaders;

};

#endif
