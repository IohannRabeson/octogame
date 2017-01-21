#ifndef TEXTMANAGER_HPP
# define TEXTMANAGER_HPP

# include <vector>
# include <map>
# include <memory>
# include "RandomGenerator.hpp"
# include "ResourceDefinitions.hpp"
# include "ABubble.hpp"

class TextManager
{
public:
	TextManager(void);
	~TextManager(void) = default;

	static TextManager & getInstance(void);
	void loadTexts(void);
	std::vector<std::wstring> const & getTexts(std::string const & key);
	std::vector<std::wstring> const & getTextsNpc(ResourceKey const & key);
	ABubble::Priority const & getPriority(ResourceKey const & key);

private:
	static std::unique_ptr<TextManager>					m_instance;
	std::map<std::string, std::vector<std::wstring>>	m_texts;
	std::map<std::string, ABubble::Priority>			m_priority;
	RandomGenerator										m_generator;
};

#endif
