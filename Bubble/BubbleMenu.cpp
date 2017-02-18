#include "BubbleMenu.hpp"
#include "Progress.hpp"

#include <Application.hpp>
#include <ResourceManager.hpp>

#include "ResourceDefinitions.hpp"

BubbleMenu::BubbleMenu(void) :
	m_characterSize(20u),
	m_isFontSelect(true),
	m_indexCursor(0u)
{
}

void BubbleMenu::setup(std::vector<std::wstring> const & subMenuTitle, sf::Color const & color, std::size_t characterSize)
{
	m_contentSize.x = 0.f;
	m_contentSize.y = 0.f;
	if (characterSize != 0u)
		m_characterSize = characterSize;
	ABubble::setColor(color);

	m_subMenus.resize(subMenuTitle.size());
	m_cursorPosition.resize(subMenuTitle.size());

	octo::ResourceManager& resources = octo::Application::getResourceManager();
	m_font = resources.getFont(CHINESETROOPS_TTF);
	m_fontSelect = resources.getFont(VERAMONO_TTF);

	for (std::size_t i = 0; i < m_subMenus.size(); i++)
	{
		m_subMenus[i].text.setFont(m_fontSelect);
		m_subMenus[i].text.setFillColor(sf::Color(0, 0, 0));
		m_subMenus[i].text.setCharacterSize(m_characterSize);
		m_subMenus[i].text.setString(subMenuTitle[i]);
		m_subMenus[i].size.x = m_subMenus[i].text.findCharacterPos(subMenuTitle[i].size() - 1).x - m_subMenus[i].text.findCharacterPos(0).x + m_font.getGlyph(subMenuTitle[i][subMenuTitle[i].size() - 1], m_characterSize, 0).advance;
		m_subMenus[i].position.y = m_contentSize.y;
		if (m_subMenus[i].size.x > m_contentSize.x)
			m_contentSize.x = m_subMenus[i].size.x;
		m_contentSize.y += m_font.getLineSpacing(m_characterSize);
	}
}

std::vector<sf::Vector2f> const & BubbleMenu::getCursorPosition(void)
{
	for (std::size_t i = 0; i < m_subMenus.size(); i++)
	{
		m_cursorPosition[i].x = m_contentSize.x + getSizeCorner() * 2.f;
		m_cursorPosition[i].y = m_subMenus[i].position.y + m_font.getLineSpacing(m_characterSize) / 2.f;
		m_subMenus[i].position.x = (m_contentSize.x - m_subMenus[i].size.x) / 2.f;
	}

	return m_cursorPosition;
}

sf::Vector2f const & BubbleMenu::getContentSize() const
{
	return m_contentSize;
}

void BubbleMenu::updateContent(sf::Time frameTime, sf::Vector2f const & position)
{
	(void)frameTime;
	for (std::size_t i = 0; i < m_subMenus.size(); i++)
	{
		m_subMenus[i].text.setPosition(position + m_subMenus[i].position);
		if (m_isFontSelect && Progress::getInstance().getMenuType() == Progress::MenuType::Classic)
		{
			if (m_indexCursor == i)
				m_subMenus[i].text.setFont(m_fontSelect);
			else
				m_subMenus[i].text.setFont(m_font);
		}
		else
			m_subMenus[i].text.setFont(m_fontSelect);
	}
}

void BubbleMenu::drawContent(sf::RenderTarget & render, sf::RenderStates states) const
{
	for (auto subMenu : m_subMenus)
		render.draw(subMenu.text, states);
}

void BubbleMenu::setIndexCursor(std::size_t index)
{
	m_indexCursor = index;
}

void BubbleMenu::setIsFontSelect(bool isFontSelect)
{
	m_isFontSelect = isFontSelect;
}
