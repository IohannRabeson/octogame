#include "ScientistJu.hpp"

ScientistJu::ScientistJu(void) :
	ScientistNpc(SCIENTISTJU_OSS)
{
	setup();
	setupBox(this, static_cast<std::size_t>(GameObjectType::Npc), static_cast<std::size_t>(GameObjectType::PlayerEvent));
	setTextOffset(sf::Vector2f(54.f, -40.f));
}
