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
#if _WIN32
		if (line.length())
			line.pop_back();
#endif
		m_texts[key].push_back(line);
	}
}

std::vector<std::wstring> const & TextManager::getTexts(std::string const & key)
{
	if (Progress::getInstance().isJoystick())
	{
		if (!(m_texts.find("joystick_" + key) == m_texts.end()))
			return (m_texts["joystick_" + key]);
	}
	return m_texts[key];
}

std::vector<std::wstring> const & TextManager::getTextsNpc(ResourceKey const & key)
{
	if (m_texts[key].size())
		m_texts[key].push_back(L"Beurk!\n");

	if (Progress::getInstance().isJoystick())
	{
		if (!(m_texts.find("joystick_" + static_cast<std::string>(key)) == m_texts.end()))
			return (m_texts["joystick_" + static_cast<std::string>(key)]);
	}
	return m_texts[key];
}
