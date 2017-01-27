#include "BubbleText.hpp"

#include <Application.hpp>
#include <ResourceManager.hpp>

#include "ResourceDefinitions.hpp"

#include <iterator>

BubbleText::BubbleText(void) :
	m_bubbleWidth(350.f),
	m_characterPerLineMax(0u),
	m_characterPerLine(0u),
	m_lineCount(1u)
{
}

void BubbleText::setupBlocString(void)
{
	std::wstring word = L"";
	std::wstring line = L"";
	std::wstring phrase = L"";
	m_lineCount = 1u;

	for (auto it = m_phrase.begin(); it != m_phrase.end(); it++)
	{
		if (*it != ' ' && *it != '\\' && *it != '\n')
			word += *it;
		else if (*it == L'\\')
		{
			line += word + L' ';
			*(line.end() - 1) = L'\n';
			if (line.size() - 1u > m_characterPerLine)
				m_characterPerLine = line.size() - 1u;
			phrase += line;
			m_lineCount += 1u;
			word = L"";
			line = L"";
		}
		else if (line.size() + word.size() >= m_characterPerLineMax)
		{
			*(line.end() - 1) = L'\n';
			if (line.size() - 1u > m_characterPerLine)
				m_characterPerLine = line.size() - 1u;
			phrase += line;
			m_lineCount += 1u;
			line = word + L' ';
			word = L"";
		}
		else
		{
			line += word + L' ';
			word = L"";
		}
	}
	if (line.size())
	{
		if (line.size() + word.size() > m_characterPerLine)
			m_characterPerLine = line.size() + word.size();
		phrase += line + word + L'\n';
	}

	m_phrase = phrase;
}

void BubbleText::setup(std::wstring const & phrase, sf::Color const & color, std::size_t characterSize, float bubbleWidth, sf::Color const & colorText)
{
	m_bubbleWidth = bubbleWidth;
	m_phrase = phrase;
	ABubble::setColor(color);
	m_contentSize.x = m_bubbleWidth;

	octo::ResourceManager& resources = octo::Application::getResourceManager();
	m_font = resources.getFont(VERAMONO_TTF);
	m_text.setFont(m_font);
	m_text.setColor(colorText);
	m_text.setCharacterSize(characterSize);
	m_text.setString(m_phrase);

	float widthTotalText = m_text.findCharacterPos(m_phrase.size() - 1).x - m_text.findCharacterPos(0).x + m_font.getGlyph(m_phrase[m_phrase.size() - 1], characterSize, 0).advance;
	m_characterPerLineMax = static_cast<std::size_t>(m_contentSize.x / (widthTotalText / m_phrase.size()));

	setupBlocString();
	m_contentSize.y = m_lineCount * m_font.getLineSpacing(characterSize);
	if (m_lineCount == 1u)
		m_contentSize.x = widthTotalText;
	else if (m_characterPerLine != m_characterPerLineMax)
		m_contentSize.x = m_characterPerLine * (widthTotalText / (m_phrase.size() - 1u));
	m_text.setString(m_phrase);
	m_contentUpdated = false;
}

sf::Vector2f const & BubbleText::getContentSize() const
{
	return m_contentSize;
}

void BubbleText::updateContent(sf::Time frameTime, sf::Vector2f const & position)
{
	(void)frameTime;
	m_contentUpdated = true;
	m_text.setPosition(position);
}

void BubbleText::drawContent(sf::RenderTarget & render, sf::RenderStates states) const
{
	if (m_contentUpdated)
		render.draw(m_text, states);
}

void BubbleText::setPhrase(std::wstring const & text)
{
	m_text.setString(text);
}

std::wstring const & BubbleText::getPhrase(void)
{
	return m_phrase;
}
