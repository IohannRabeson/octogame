#include "BirdBlueNpc.hpp"

BirdBlueNpc::BirdBlueNpc(void) :
	BirdNpc(BIRD_BLUE_OSS)
{
	setupBox(this, static_cast<std::size_t>(GameObjectType::LucienNpc), static_cast<std::size_t>(GameObjectType::PlayerEvent));
}