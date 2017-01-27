#include "SpiritInfos.hpp"
#include "TextManager.hpp"
#include "Progress.hpp"

SpiritInfos::SpiritInfos(void) :
	m_missingText(L"missing text"),
	m_generator("random")
{
}

void SpiritInfos::setup(void)
{
	m_progressionBubble.resize(4);

	for (std::size_t i = 0u; i < m_progressionBubble.size(); i++)
	{
		BubbleText * bubble = new BubbleText();
		m_progressionBubble[i] = std::unique_ptr<BubbleText>(bubble);
		m_progressionBubble[i]->setType(ABubble::Type::Speak);
		m_progressionBubble[i]->setActive(true);
	}
}

void SpiritInfos::update(sf::Time frameTime, sf::Vector2f const & position)
{
	updateNanoRobots(frameTime, position);
	updateProgression(frameTime, position);
}

void SpiritInfos::updateNanoRobots(sf::Time frameTime, sf::Vector2f const & position)
{
	for (std::size_t i = 0; i < Progress::getInstance().getSpiritCount() + 1u; i++)
	{
		if (!Progress::getInstance().isMenu())
		{
			if (m_spirit.size() != Progress::getInstance().getSpiritCount() + 1u)
			{
				NanoRobot * spirit = new MenuNanoRobot(position + sf::Vector2f(m_generator.randomFloat(-400.f, 400.f), m_generator.randomFloat(-400.f, 400.f)));
				m_spirit.push_back(std::unique_ptr<NanoRobot>(spirit));
			}
			else if (i < m_spirit.size())
			{
				m_spirit[i]->setState(NanoRobot::State::FollowOcto);
				m_spirit[i]->setPosition(position);
				m_spirit[i]->update(frameTime);
			}
		}
	}
}

void SpiritInfos::updateProgression(sf::Time frameTime, sf::Vector2f const & position)
{
	Progress & progress = Progress::getInstance();
	std::size_t current = 0u;
	std::size_t max = 0u;
	std::string key = "";
	std::size_t size = 16u;
	float dist = 0.f;

	for (std::size_t i = 0u; i < m_progressionBubble.size(); i++)
	{
		switch (i)
		{
			case 0u:
				current = progress.getProgression();
				max = static_cast<std::size_t>(Level::Final) + 1u;
				key = "progression_level";
				break;
			case 1u:
				current = progress.countRandomDiscover();
				max = Progress::RandomPortalMax;
				key = "progression_portal";
				break;
			case 2u:
				current = progress.getNanoRobotCount();
				max = 7u;
				key = "progression_octobot";
				break;
			case 3u:
				current = progress.getCheckpointCount();
				max = progress.getCheckpointCountMax();
				key = "progression_checkpoint";
			default:
				break;
		}
	
		if (current > 1u)
			key += "_plural";
		else
			key += "_singular";
		m_progressionString = std::to_wstring(current) + L" " + getText("progression_of") + L" " + std::to_wstring(max) + L" " + getText(key);
		if (i < m_spirit.size())
		{
			dist = std::sqrt(std::pow(m_spirit[i]->getPosition().x - position.x, 2u) + std::pow(m_spirit[i]->getPosition().y - position.y, 2u));
			m_progressionBubble[i]->setPosition(m_spirit[i]->getPosition() - sf::Vector2f(10.f, 16.f));
		}
		float alphaCoef = std::max((1.f - std::max(500.f - dist + 100.f, 0.f) / 500.f), 0.f);
		m_progressionBubble[i]->setup(m_progressionString, sf::Color(255, 255, 255, 70.f * alphaCoef), size, 120.f, sf::Color(0, 0, 0, 255 * alphaCoef));
		m_progressionBubble[i]->update(frameTime);
	}
}

void SpiritInfos::draw(sf::RenderTarget & render, sf::RenderStates states) const
{
	if (!Progress::getInstance().isMenu())
	{
		for (std::size_t i = 0u; i < m_spirit.size(); i++)
		{
			 if (m_spirit[i])
				m_spirit[i]->draw(render, states);
		}
		for (std::size_t i = 0u; i < m_progressionBubble.size(); i++)
			m_progressionBubble[i]->draw(render, states);
	}
}

std::wstring const & SpiritInfos::getText(std::string const & text)
{
	std::vector<std::wstring> const & menuText = TextManager::getInstance().getTexts(text);
	if (menuText.size())
		return menuText[0];
	return m_missingText;
}
