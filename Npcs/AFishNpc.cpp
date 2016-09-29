#include "AFishNpc.hpp"
#include "RectangleShape.hpp"
#include <Interpolations.hpp>

AFishNpc::AFishNpc(ResourceKey const & npcId, ABiome & biome, bool isMeetable) :
	ASwimNpc(npcId, biome, isMeetable)
{
	setScale(randomFloat(0.5f, 0.9f));
	setTextOffset(sf::Vector2f(0.f, 0.f));
	setFollowOcto(false);
	reverseSprite(true);
}

void AFishNpc::update(sf::Time frametime)
{
	octo::CharacterSprite & sprite = getSprite();
	RectangleShape * box = getBox();
	float angle = octo::rad2Deg(std::atan2(box->getPosition().y - getOctoPosition().y, box->getPosition().x - getOctoPosition().x));

	if (sprite.getCurrentEvent() == Special1)
	{
		if (angle < 0.f)
			angle += 360.f;
		sprite.setRotation(angle);
	}
	else
	{
		if (sprite.getRotation() > 90.f && sprite.getRotation() < 270.f)
			sprite.setRotation(octo::linearInterpolation(180.f, sprite.getRotation(), 1.f - frametime.asSeconds()));
		else
			sprite.setRotation(octo::linearInterpolation(0.f, sprite.getRotation(), 1.f - frametime.asSeconds()));
	}

	if (angle > 90.f && angle < 270.f)
		upSideDownSprite(true);
	else
		upSideDownSprite(false);

	ASwimNpc::update(frametime);
}
