#include "BirdRedNpc.hpp"

BirdRedNpc::BirdRedNpc(void) :
	AFlyNpc(BIRD_RED_OSS)
{
	setupBox(this, static_cast<std::size_t>(GameObjectType::Npc), static_cast<std::size_t>(GameObjectType::PlayerEvent));
}
