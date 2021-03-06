#include "ScientistFran.hpp"

ScientistFran::ScientistFran(void) :
	ScientistNpc(SCIENTISTFRAN_OSS)
{
	setup();
	setupBox(this, static_cast<std::size_t>(GameObjectType::Npc), static_cast<std::size_t>(GameObjectType::PlayerEvent));
	setTextOffset(sf::Vector2f(50.f, -50.f));
}
