#include "ForestSpirit1Npc.hpp"

ForestSpirit1Npc::ForestSpirit1Npc(void) :
	DisappearNpc(FOREST_SPIRIT_1_OSS)
{
	setupBox(this, static_cast<std::size_t>(GameObjectType::Npc), static_cast<std::size_t>(GameObjectType::PlayerEvent));
}
