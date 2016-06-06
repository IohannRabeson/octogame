#include "ForestSpirit2Npc.hpp"

ForestSpirit2Npc::ForestSpirit2Npc(void) :
	DisappearNpc(FOREST_SPIRIT_2_OSS)
{
	setupBox(this, static_cast<std::size_t>(GameObjectType::Npc), static_cast<std::size_t>(GameObjectType::PlayerEvent));
}
