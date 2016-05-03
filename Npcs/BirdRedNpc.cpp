#include "BirdRedNpc.hpp"

BirdRedNpc::BirdRedNpc(void) :
	BirdNpc(BIRD_RED_OSS)
{
	setupBox(this, static_cast<std::size_t>(GameObjectType::LucienNpc), static_cast<std::size_t>(GameObjectType::PlayerEvent));
}
