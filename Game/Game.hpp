#ifndef GAME_HPP
# define GAME_HPP

# include "CharacterNpc.hpp"
# include "SkyCycle.hpp"
# include "BiomeManager.hpp"
# include "SkyManager.hpp"
# include "GroundManager.hpp"
# include "ParallaxScrolling.hpp"
# include "CharacterOcto.hpp"
# include "PhysicsEngine.hpp"
# include "IContactListener.hpp"
# include "MusicPlayer.hpp"

class PhysicsEngine;
class AShape;

class Game : public octo::DefaultKeyboardListener, public IContactListener
{
public:
	Game();

	void	setup();
	void	loadLevel(std::string const& fileName);

	void			update(sf::Time frameTime);
	void			draw(sf::RenderTarget& render, sf::RenderStates states)const;

private:
	PhysicsEngine &		m_physicsEngine;
	SkyCycle			m_skyCycle;
	BiomeManager		m_biomeManager;
	SkyManager			m_skyManager;
	GroundManager		m_groundManager;
	ParallaxScrolling	m_parallaxScrolling;
	MusicPlayer			m_musicPlayer;
	CharacterOcto		m_octo;
	sf::Vector2f		m_cameraPos;
	sf::Vector2f		m_octoPos;
	CharacterNpc		m_npc;

	bool			onPressed(sf::Event::KeyEvent const & event);
	void			onShapeCollision(AShape * shapeA, AShape * shapeB);
	void			onTileShapeCollision(TileShape * tileShape, AShape * shape);
	void			followPlayer();
};

#endif
