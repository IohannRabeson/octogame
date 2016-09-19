#ifndef GAME_HPP
# define GAME_HPP

# include "SkyCycle.hpp"
# include "BiomeManager.hpp"
# include "SkyManager.hpp"
# include "GroundManager.hpp"
# include "ParallaxScrolling.hpp"
# include "CharacterOcto.hpp"
# include "PhysicsEngine.hpp"
# include "IContactListener.hpp"
# include "MusicManager.hpp"
# include "KonamiCode.hpp"
# include "FakeMenu.hpp"

#include "InputListener.hpp"

# include <memory>

class PhysicsEngine;
class AShape;
class CameraMovement;

class Game : public InputListener, public IContactListener
{
public:
	Game(void);
	virtual ~Game(void);

	void			loadLevel(void);
	sf::Vector2f	getOctoBubblePosition(void) const;

	void			update(sf::Time frameTime);
	void			draw(sf::RenderTarget& render, sf::RenderStates states)const;

private:
	PhysicsEngine &						m_physicsEngine;
	MusicManager &						m_musicPlayer;
	BiomeManager						m_biomeManager;
	std::unique_ptr<SkyCycle>			m_skyCycle;
	std::unique_ptr<SkyManager>			m_skyManager;
	std::unique_ptr<GroundManager>		m_groundManager;
	std::unique_ptr<ParallaxScrolling>	m_parallaxScrolling;
	std::unique_ptr<CharacterOcto>		m_octo;
	std::unique_ptr<KonamiCode>			m_konami;
	std::unique_ptr<CameraMovement>		m_cameraMovement;
	bool								m_keyGroundRight;
	bool								m_keyGroundLeft;
	bool								m_keyInfos;
	std::shared_ptr<sf::Sound>			m_soundGeneration;
	float								m_groundVolume;
	sf::Time							m_groundSoundTime;
	sf::Time							m_groundSoundTimeMax;
	bool								m_isSlowTime;
	sf::Time							m_slowTime;
	sf::Time							m_slowTimeMax;
	float								m_slowSource;
	float								m_slowTarget;
	float								m_slowTimeCoef;
	std::size_t							m_skipFrames;
	std::size_t							m_skipFramesMax;
	FakeMenu							m_fakeMenu;

	void			updateSlowTime(sf::Time frameTime);
	void			updateFakeMenu(sf::Time frameTime);
	void			moveMap(sf::Time frameTime);
	bool			onInputPressed(InputListener::OctoKeys const & key);
	bool			onInputReleased(InputListener::OctoKeys const & key);
	void			onShapeCollision(AShape * shapeA, AShape * shapeB, sf::Vector2f const & collisionDirection);
	void			onTileShapeCollision(TileShape * tileShape, AShape * shape, sf::Vector2f const & collisionDirection);
	void			followPlayer(sf::Time frameTime);
	void			onCollision(CharacterOcto * octo, AGameObjectBase * gameObject, sf::Vector2f const & collisionDirection);
	void			onCollisionEvent(CharacterOcto * octo, AGameObjectBase * gameObject, sf::Vector2f const & collisionDirection);
};

#endif
