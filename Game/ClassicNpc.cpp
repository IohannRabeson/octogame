#include "ClassicNpc.hpp"

ClassicNpc::ClassicNpc(ResourceKey const & npcId) :
	ANpc(npcId)
{
	setSize(sf::Vector2f(50.f, 150.f));
	setOrigin(sf::Vector2f(65.f, 0.f));
	setup();
}

void ClassicNpc::setup(void)
{
	setupIdleAnimation({
			FramePair(0.4f, 10u),
			FramePair(0.4f, 11u),
			FramePair(0.4f, 12u),
			FramePair(0.4f, 13u),
			FramePair(0.4f, 14u),
			FramePair(0.4f, 15u),
			FramePair(0.4f, 16u)}, octo::LoopMode::Loop);
	setupWalkAnimation({
			FramePair(0.4f, 0u),
			FramePair(0.4f, 1u),
			FramePair(0.4f, 2u),
			FramePair(0.4f, 3u),
			FramePair(0.4f, 4u),
			FramePair(0.4f, 5u),
			FramePair(0.4f, 6u)}, octo::LoopMode::Loop);
	setupMachine();
}
