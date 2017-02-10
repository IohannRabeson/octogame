#include "ScientistLu.hpp"

ScientistLu::ScientistLu(void) :
	ScientistNpc(SCIENTISTLU_OSS)
{
	setup();
	setupBox(this, static_cast<std::size_t>(GameObjectType::Npc), static_cast<std::size_t>(GameObjectType::PlayerEvent));
	setTextOffset(sf::Vector2f(50.f, -50.f));
}

void ScientistLu::updateText(sf::Time frametime)
{
	if (!m_updateText)
		return;

	auto it = m_indexText.find(m_currentIndex);
	if (it == m_indexText.end())
	{
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
