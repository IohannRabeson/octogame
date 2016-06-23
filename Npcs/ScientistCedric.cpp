#include "ScientistCedric.hpp"

ScientistCedric::ScientistCedric(void) :
	ScientistNpc(SCIENTISTCEDRIC_OSS)
{
	setupBox(this, static_cast<std::size_t>(GameObjectType::Npc), static_cast<std::size_t>(GameObjectType::PlayerEvent));
}
