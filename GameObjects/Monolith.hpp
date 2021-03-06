#ifndef MONOLITH_HPP
# define MONOLITH_HPP

# include "InstanceDecor.hpp"
# include "AGameObject.hpp"
# include <CharacterSprite.hpp>
# include <SFML/Graphics/RectangleShape.hpp>
# include <VertexBuilder.hpp>
# include <memory>

class MonolithStep;
class CircleShape;
class CharacterOcto;
class FinalPortal;
class ABiome;

class Monolith : public InstanceDecor, public AGameObject<GameObjectType::Monolith>
{
public:
	Monolith(sf::Vector2f const & scale, sf::Vector2f const & position, ABiome & biome);
	virtual ~Monolith(void);

	virtual void addMapOffset(float x, float y);
	virtual void setPosition(sf::Vector2f const & position);

	void collideOcto(CharacterOcto * octo);
	void collideOctoEvent(CharacterOcto * octo);
	virtual void update(sf::Time frameTime);
	virtual bool isInScreen(void);
	virtual void draw(sf::RenderTarget& render, sf::RenderStates states) const;
	virtual void drawFront(sf::RenderTarget& render, sf::RenderStates states) const;

private:
	enum State
	{
		Wait,
		StartEffect,
		Activate,
		StartFinalScene,
		FinalLosange,
		MoveAtFinalPosition,
		CircleExplosionAndRedEffect,
		WhiteFlash,
		FinalExplosion,
		Implode,
		WhiteFlash2,
		None
	};

	enum SpriteIndex
	{
		DownLeft2 = 0,
		Up = 1,
		DownLeft1 = 2,
		DownRight = 3,
		Gemme = 4,
		FissureWhite = 5,
		FissureRed = 6,
		Light = 7
	};

	std::vector<octo::AnimatedSprite>			m_spriteMonolith;
	std::vector<octo::SpriteAnimation>			m_animationMonolith;
	octo::AnimatedSprite						m_spriteBalleNanoRobot;
	octo::SpriteAnimation						m_animationBalleNanoRobot;
	std::vector<sf::Vector2f>					m_position;
	std::vector<sf::Vector2f>					m_endPosition;
	std::unique_ptr<FinalPortal>				m_portal;
	std::vector<std::unique_ptr<MonolithStep>>	m_steps;
	std::unique_ptr<sf::Vertex[]>				m_vertices;
	octo::VertexBuilder							m_builder;
	sf::Vector2f								m_size;
	CircleShape *								m_box;
	CircleShape *								m_stopMovingMapBox;
	CharacterOcto *								m_octo;
	sf::RectangleShape							m_whiteForeground;
	sf::Time									m_timer;
	sf::Time									m_timerMax;
	sf::Time									m_waitBeforeStartFinalDuration;
	sf::Time									m_explosionShaderDuration;
	sf::Time									m_redFissureAppearDuration;
	sf::Time									m_whiteFlashDuration;
	sf::Time									m_moveAtFinalPositionDuration;
	sf::Time									m_forceMapMoveDuration;
	sf::Time									m_implodeDuration;
	sf::Time									m_nanoScaleDuration;
	sf::Vector2f								m_center;
	std::size_t									m_used;
	State										m_state;
	float										m_offset;
	float										m_transitionStartEndPosition;
	sf::Vector2f								m_lastPosition;

	void createLosange(sf::Vector2f const & size,
											sf::Vector2f const & origin,
											sf::Color const & color,
											octo::VertexBuilder& builder);

	void createEffect(sf::Vector2f const & size,
											sf::Vector2f const & origin,
											float glowingCoef,
											sf::Color color,
											octo::VertexBuilder& builder);

};

#endif
