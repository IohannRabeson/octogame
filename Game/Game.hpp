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

# include <memory>

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
	PhysicsEngine &						m_physicsEngine;
	BiomeManager						m_biomeManager;
	std::unique_ptr<SkyCycle>			m_skyCycle;
	std::unique_ptr<SkyManager>			m_skyManager;
	std::unique_ptr<GroundManager>		m_groundManager;
	std::unique_ptr<ParallaxScrolling>	m_parallaxScrolling;
	MusicPlayer							m_musicPlayer;
	std::unique_ptr<CharacterOcto>		m_octo;
	std::unique_ptr<CharacterNpc>		m_npc; //TODO: remove

	bool			onPressed(sf::Event::KeyEvent const & event);
	void			onShapeCollision(AShape * shapeA, AShape * shapeB, sf::Vector2f const & collisionDirection);
	void			onTileShapeCollision(TileShape * tileShape, AShape * shape, sf::Vector2f const & collisionDirection);
	void			followPlayer(sf::Time frameTime);
};

#endif
