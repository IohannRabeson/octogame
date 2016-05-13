#include "TextManager.hpp"
#include "Progress.hpp"
#include <Application.hpp>
#include <ResourceManager.hpp>
#include <sstream>
#include <cwchar>

std::unique_ptr<TextManager> TextManager::m_instance = nullptr;

TextManager & TextManager::getInstance()
{
	if (m_instance == nullptr)
	{
		m_instance.reset(new TextManager());
	}
	return *m_instance;
}

TextManager::TextManager(void)
{
	loadTexts();
}

void TextManager::loadTexts(void)
{
	octo::ResourceManager &	resources = octo::Application::getResourceManager();
	Progress &				progress = Progress::getInstance();

	m_texts.clear();
	std::wstringstream f(resources.getText(progress.getTextFile()).toWideString());
	std::wstring wkey;
	std::wstring line;
	wchar_t delim = '=';
	while (std::getline(f, wkey, delim))
	{
		std::getline(f, line);
		std::string key(wkey.begin(), wkey.end());
		m_texts[key].push_back(line);
	}
}

std::vector<std::wstring> const & TextManager::getTexts(std::string const & key)
{
	return m_texts[key];
}

std::vector<std::wstring> const & TextManager::getTextsNpc(ResourceKey const & key)
{
	if (m_texts[key].size())
		m_texts[key].push_back(L"Beurk!");
	return m_texts[key];
}
