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

TextManager::TextManager(void) :
	m_generator("random")
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
		if (line.size() && line[0] == L'!')
		{
			m_priority[key] = ABubble::Priority::Important;
			line.erase(0, 1);
		}
		else if (line.size() && line[0] == L'\"')
		{
			m_priority[key] = ABubble::Priority::Tips;
			line.erase(0, 1);
		}
		else
			m_priority[key] = ABubble::Priority::Bullshit;
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

std::vector<std::wstring> TextManager::getTextsNpc(ResourceKey const & key)
{
	Progress const & progress = Progress::getInstance();
	std::vector<std::wstring> texts;

	texts = m_texts[key];
	if (Progress::getInstance().isJoystick())
	{
		if (!(m_texts.find("joystick_" + static_cast<std::string>(key)) == m_texts.end()))
			texts = m_texts["joystick_" + static_cast<std::string>(key)];
	}
	if (progress.getLastDestination() == Level::Portal && !progress.isMenu() && m_texts[key].size())
	{
		std::size_t random = m_generator.randomInt(0u, 3u);
		std::wstring text;

		switch (random)
		{
			case 0:
				texts = m_texts["finished_game_0"];
				break;
			case 1:
				texts = m_texts["finished_game_1"];
				break;
			case 2:
				texts = m_texts["finished_game_2"];
				break;
			case 3:
				texts = m_texts["finished_game_3"];
				break;
			default:
				break;
		}
	}

	if (texts.size())
		texts.push_back(L"Beurk!\n");

	return texts;
}

ABubble::Priority const & TextManager::getPriority(ResourceKey const & key)
{
	return m_priority[key];
}
