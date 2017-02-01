#include "SpiritInfos.hpp"
#include "TextManager.hpp"
#include "Progress.hpp"

SpiritInfos::SpiritInfos(void) :
	m_progressionCount(7u),
	m_missingText(L"missing text"),
	m_generator("random")
{
}

void SpiritInfos::setup(void)
{
	m_progressionBubbles.resize(m_progressionCount);

	for (std::size_t i = 0u; i < m_progressionBubbles.size(); i++)
	{
		BubbleText * bubble = new BubbleText();
		m_progressionBubbles[i] = std::unique_ptr<BubbleText>(bubble);
		m_progressionBubbles[i]->setType(ABubble::Type::Speak);
		m_progressionBubbles[i]->setActive(true);
	}

	setupSpirit(Progress::getInstance().getOctoPos());
	setupProgression();
}

void SpiritInfos::setupSpirit(sf::Vector2f const & position)
{
	for (std::size_t i = 0; i < Progress::getInstance().getSpiritCount() + 1u; i++)
	{
		if (m_spirits.size() != Progress::getInstance().getSpiritCount() + 1u)
		{
			NanoRobot * spirit = new MenuNanoRobot(position + getRandomVector2f());
			spirit->setState(NanoRobot::State::FollowOcto);
			m_spirits.push_back(std::unique_ptr<NanoRobot>(spirit));
		}
		m_spirits[i]->setHardPosition(position + getRandomVector2f());
		m_spirits[i]->setPosition(position + getRandomVector2f());
	}
}

sf::Vector2f SpiritInfos::getRandomVector2f(void)
{
	sf::Vector2f position;

	if (m_generator.randomBool(0.5f))
		position.x = m_generator.randomFloat(-400.f, -200.f);
	else
		position.x = m_generator.randomFloat(400.f, 200.f);
	if (m_generator.randomBool(0.5f))
		position.y = m_generator.randomFloat(-400.f, -200.f);
	else
		position.y = m_generator.randomFloat(400.f, 200.f);

	return position;
}

void SpiritInfos::setupProgression(void)
{
	Progress & progress = Progress::getInstance();
	std::size_t current = 0u;
	std::size_t max = 0u;
	std::string key = "";
	std::size_t size = 16u;

	for (std::size_t i = 0u; i < m_progressionBubbles.size(); i++)
	{
		if (i * 2u < m_spirits.size())
		{
			max = 0u;
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
					max = 8u;
					key = "progression_octobot";
					break;
				case 3u:
					current = progress.getCheckpointCount();
					max = progress.getCheckpointCountMax();
					key = "progression_checkpoint";
					break;
				case 4u:
					current = progress.getTimePlayed();
					key = "progression_time";
					break;
				case 5u:
					current = progress.getDeathCount();
					key = "progression_death";
					break;
				case 6u:
					current = progress.getNpcCount();
					max = 60u;
					key = "progression_npc";
					break;
				default:
					break;
			}
		
			if (current > 1u)
				key += "_plural";
			else
				key += "_singular";
	
			if (max != 0u)
				m_progressionString = std::to_wstring(current) + L" " + getText("progression_of") + L" " + std::to_wstring(max) + L" " + getText(key);
			else
				m_progressionString = std::to_wstring(current) + L" " + getText(key);
	
			m_progressionBubbles[i]->setup(m_progressionString, sf::Color(255, 255, 255, 0), size, 350.f, sf::Color(0, 0, 0, 0));
		}
	}
}

void SpiritInfos::update(sf::Time frameTime, sf::Vector2f const & position)
{
	updateSpirit(frameTime, position);
	updateProgression(frameTime, position);
}

void SpiritInfos::updateSpirit(sf::Time frameTime, sf::Vector2f const & position)
{
	for (std::size_t i = 0; i < Progress::getInstance().getSpiritCount() + 1u; i++)
	{
		if (!Progress::getInstance().isMenu() && i < m_spirits.size())
		{
			m_spirits[i]->setState(NanoRobot::State::FollowOcto);
			m_spirits[i]->setPosition(position);
			m_spirits[i]->update(frameTime);
		}
	}
}

void SpiritInfos::updateProgression(sf::Time frameTime, sf::Vector2f const & position)
{
	float dist = 0.f;

	for (std::size_t i = 0u; i < m_progressionBubbles.size(); i++)
	{
		if (i * 2u < m_spirits.size())
		{
			dist = std::sqrt(std::pow(m_spirits[i * 2u]->getPosition().x - position.x, 2u) + std::pow(m_spirits[i * 2u]->getPosition().y - position.y, 2u));
			float alphaCoef = std::max((1.f - std::max(500.f - dist + 100.f, 0.f) / 500.f), 0.f);

			m_progressionBubbles[i]->setPosition(m_spirits[i * 2u]->getPosition() - sf::Vector2f(10.f, 16.f));
			m_progressionBubbles[i]->setColors(sf::Color(255, 255, 255, 70.f * alphaCoef), sf::Color(0, 0, 0, 255 * alphaCoef));
			m_progressionBubbles[i]->update(frameTime);
		}
	}
}

void SpiritInfos::draw(sf::RenderTarget & render, sf::RenderStates states) const
{
	if (!Progress::getInstance().isMenu())
	{
		for (std::size_t i = 0u; i < m_spirits.size(); i++)
		{
			 if (m_spirits[i])
				m_spirits[i]->draw(render, states);
		}
		for (std::size_t i = 0u; i < m_progressionBubbles.size(); i++)
			m_progressionBubbles[i]->draw(render, states);
	}
}

std::wstring const & SpiritInfos::getText(std::string const & text)
{
	std::vector<std::wstring> const & menuText = TextManager::getInstance().getTexts(text);
	if (menuText.size())
		return menuText[0];
	return m_missingText;
}
