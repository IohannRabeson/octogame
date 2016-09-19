#include "AUniqueNpc.hpp"

AUniqueNpc::AUniqueNpc(ResourceKey const & npcId, bool isMeetable) :
	ANpc(npcId, isMeetable)
{
	setupBox(this, static_cast<std::size_t>(GameObjectType::Npc), static_cast<std::size_t>(GameObjectType::PlayerEvent));
}
