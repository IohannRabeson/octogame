#include "ScientistNpc.hpp"
#include "RectangleShape.hpp"
#include "TextManager.hpp"
#include <sstream>
#include <vector>

ScientistNpc::ScientistNpc(ResourceKey key) :
	ANpc(key),
	m_currentIndex(0u),
	m_updateText(false)
{
	setSize(sf::Vector2f(25.f, 140.f));
	setOrigin(sf::Vector2f(90.f, 100.f));
	setScale(1.0f);
	setTimerMax(sf::seconds(8.0f));

	getBox()->setApplyGravity(false);

	TextManager & textManager = TextManager::getInstance();
	std::vector<std::wstring> const & texts = textManager.getTextsNpc(key);
	std::vector<std::wstring> newTexts;
	getTexts().clear();
	for (std::size_t i = 0u; i < texts.size(); i++)
	{
		std::vector<std::wstring> tokens;
		std::wstringstream ss(texts[i]);
		std::wstring item;
		while (std::getline(ss, item, L'#'))
			tokens.push_back(item);
		if (tokens.size() == 2)
		{
			newTexts.push_back(tokens[1] + L"\n");
			m_indexText[std::stoi(tokens[0])] = i;
		}
	}
	setTexts(newTexts, textManager.getPriority(key));
}

void ScientistNpc::setup(void)
{
	typedef octo::CharacterAnimation::Frame			Frame;

	getIdleAnimation().setFrames({
			Frame(sf::seconds(0.4f), {0u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {1u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {2u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {3u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {2u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {1u, sf::FloatRect(), sf::Vector2f()}),
			});
	getIdleAnimation().setLoop(octo::LoopMode::Loop);

	setupMachine();
}

void ScientistNpc::setupMachine(void)
{
	typedef octo::CharacterSprite::ACharacterState	State;
	typedef octo::FiniteStateMachine::StatePtr		StatePtr;

	octo::FiniteStateMachine	machine;
	StatePtr					idleState;

	idleState = std::make_shared<State>("0", getIdleAnimation(), getSprite());

	machine.setStart(idleState);
	machine.addTransition(Idle, idleState, idleState);

	setMachine(machine);
	setNextEvent(Idle);
}

void ScientistNpc::updateText(sf::Time frametime)
{
	if (!m_updateText)
		return;

	auto it = m_indexText.find(m_currentIndex);
	if (it == m_indexText.end())
	{
		setDisplayText(false);
		return;
	}
	setDisplayText(true);
	std::size_t index = it->second;
	setCurrentText(index);
	auto & texts = ANpc::getTexts();
	texts[index]->setActive(true);
	texts[index]->setPosition(getPosition() + sf::Vector2f(50.f, -40.f));
	texts[index]->update(frametime);
}

void ScientistNpc::updateState(void)
{}

void ScientistNpc::setTextIndex(std::size_t index)
{
	m_currentIndex = index;
}

std::size_t ScientistNpc::getLastIndex(void) const
{
	std::size_t max = 0u;
	for (auto const & index : m_indexText)
		max = std::max(max, index.first);
	return (max);
}

void ScientistNpc::updateText(bool update)
{
	m_updateText = update;
}
