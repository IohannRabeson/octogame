#include "Credit.hpp"
#include "TextManager.hpp"
#include <Application.hpp>
#include <ResourceManager.hpp>

Credit::Credit(sf::Vector2f const & position) :
	m_speed(0.f, -150.f),
	m_position(position)
{
	m_font = octo::Application::getResourceManager().getFont(VERAMONO_TTF);
	appendNames(SCIENTISTFRAN_OSS);
	appendCategories(SCIENTISTLU_OSS);
}

void Credit::appendNames(ResourceKey key)
{
	TextManager & textManager = TextManager::getInstance();
	std::vector<std::wstring> const & texts = textManager.getTextsNpc(key);
	float characterSize = 20.f;

	for (std::size_t i = 0u; i < texts.size(); i++)
	{
		std::vector<std::wstring> tokens;
		std::wstringstream ss(texts[i]);
		std::wstring item;
		while (std::getline(ss, item, L'#'))
			tokens.push_back(item);
		if (tokens.size() == 2)
		{
			if (std::stoi(tokens[0]) <= 5 || std::stoi(tokens[0]) >= 78)
				continue;
			sf::Text newText;
			newText.setFont(m_font);
			newText.setCharacterSize(characterSize);
			newText.setString(tokens[1]);
			if (tokens[1].find(' ') != std::string::npos)
			{
				float widthTotalText = newText.findCharacterPos(tokens[1].find(' ') - 1).x - newText.findCharacterPos(0).x + m_font.getGlyph(tokens[1][tokens[1].size() - 1], characterSize, 0).advance;
				newText.setPosition(m_position + sf::Vector2f(0.f - widthTotalText, i * 30.f));
				m_texts.emplace(std::stoi(tokens[0]), newText);
			}
		}
	}
}

void Credit::appendCategories(ResourceKey key)
{
	TextManager & textManager = TextManager::getInstance();
	std::vector<std::wstring> const & texts = textManager.getTextsNpc(key);
	std::vector<int> index;
	float characterSize = 28.f;

	for (std::size_t i = 0u; i < texts.size(); i++)
	{
		std::vector<std::wstring> tokens;
		std::wstringstream ss(texts[i]);
		std::wstring item;
		while (std::getline(ss, item, L'#'))
			tokens.push_back(item);
		if (tokens.size() == 2)
		{
			if (std::stoi(tokens[0]) <= 5 || std::stoi(tokens[0]) >= 78)
				continue;
			tokens[1].erase(tokens[1].size() - 2, 2);
			sf::Text newText;
			newText.setFont(m_font);
			newText.setCharacterSize(characterSize);
			newText.setString(tokens[1]);
			float widthTotalText = newText.findCharacterPos(tokens[1].size() - 1).x - newText.findCharacterPos(0).x + m_font.getGlyph(tokens[1][tokens[1].size() - 1], characterSize, 0).advance;
			newText.setPosition(m_position + sf::Vector2f(0.f - widthTotalText / 2.f, i * 30.f));
			m_texts.emplace(std::stoi(tokens[0]), newText);
			index.push_back(std::stoi(tokens[0]));
		}
	}

	float offset = 0.f;
	for (auto & text : m_texts)
	{
		bool isTitle = false;
		if (index.front() == text.first)
		{
			index.erase(index.begin());
			isTitle = true;
		}
		if (isTitle)
			offset += 60.f;
		text.second.setPosition(text.second.getPosition().x, m_position.y + offset);
		if (isTitle)
			offset += 30.f;
		offset += 30.f;
	}
}

bool Credit::isFinished(void) const
{
	return (m_texts.rbegin()->second.getPosition().y < -100.f);
}

void Credit::update(sf::Time frameTime)
{
	for (auto & text : m_texts)
		text.second.setPosition(text.second.getPosition() + m_speed * frameTime.asSeconds());
}

void Credit::draw(sf::RenderTarget & render, sf::RenderStates states) const
{
	for (auto & text : m_texts)
		render.draw(text.second, states);
}
