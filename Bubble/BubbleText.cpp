#include "BubbleText.hpp"

#include <Application.hpp>
#include <ResourceManager.hpp>

#include "ResourceDefinitions.hpp"

BubbleText::BubbleText(void) :
	m_characterPerLine(0u),
	m_lineCount(1u)
{
}

void BubbleText::setupBlocString(void)
{
	std::size_t j = 0u;
	m_lineCount = 1u;

	auto lastSpace = m_phrase.begin();
	for (auto it = m_phrase.begin(); it != m_phrase.end(); it++)
	{
		j++;
		if (*it == ' ' || *it == '\\')
			lastSpace = it;
		if (j == m_characterPerLine)
		{
			*lastSpace = '\n';
			it = ++lastSpace;
			j = 0u;
			m_lineCount++;
		}
		else if (*it == 'n' && *lastSpace == '\\')
		{
			*lastSpace = '\n';
			m_phrase.erase(it);
			it = ++it;
			j = 0u;
			m_lineCount++;
		}
	}
}

void BubbleText::setup(std::wstring const & phrase, sf::Color const & color, std::size_t characterSize)
{
	if (characterSize == 0u)
		characterSize = m_characterSize;
	m_phrase = phrase;
	ABubble::setColor(color);
	m_contentSize.x = m_bubbleWidth;

	octo::ResourceManager& resources = octo::Application::getResourceManager();
	m_font = resources.getFont(VERAMONO_TTF);
	m_text.setFont(m_font);
	m_text.setColor(sf::Color(0, 0, 0));
	m_text.setCharacterSize(characterSize);
	m_text.setString(m_phrase);

	float widthTotalText = m_text.findCharacterPos(m_phrase.size() - 1).x - m_text.findCharacterPos(0).x;
	m_characterPerLine = static_cast<std::size_t>(m_contentSize.x / (widthTotalText / m_phrase.size()));

	setupBlocString();
	m_contentSize.y = m_lineCount * m_font.getLineSpacing(characterSize);
	if (m_lineCount == 1u)
		m_contentSize.x = widthTotalText + m_font.getGlyph(m_phrase[m_phrase.size() - 1], characterSize, 0).advance;
	m_text.setString(m_phrase);
}

sf::Vector2f const & BubbleText::getContentSize() const
{
	return m_contentSize;
}

void BubbleText::updateContent(sf::Time frameTime, sf::Vector2f const & position)
{
	(void)frameTime;
	m_text.setPosition(position);
}

void BubbleText::drawContent(sf::RenderTarget & render, sf::RenderStates states) const
{
	render.draw(m_text, states);
}
