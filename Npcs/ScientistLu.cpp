#include "ScientistLu.hpp"

ScientistLu::ScientistLu(void) :
	ScientistNpc(SCIENTISTLU_OSS)
{
	setup();
	setupBox(this, static_cast<std::size_t>(GameObjectType::Npc), static_cast<std::size_t>(GameObjectType::PlayerEvent));
}
