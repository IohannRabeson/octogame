#include "OverCoolNpc.hpp"

OverCoolNpc::OverCoolNpc(void) :
	BirdNpc(OVER_COOL_NPC_OSS)
{
	setupBox(this, static_cast<std::size_t>(GameObjectType::Npc), static_cast<std::size_t>(GameObjectType::PlayerEvent));
}
