#ifndef GAME_HPP
# define GAME_HPP

# include "GameClock.hpp"
# include "BiomeManager.hpp"
# include "SkyManager.hpp"
# include "GroundManager.hpp"
# include "ParallaxScrolling.hpp"
# include "CharacterOcto.hpp"
# include "CharacterNpc.hpp"
# include "CharacterBasicNpc.hpp"
# include "IContactListener.hpp"

class PhysicsEngine;
class AShape;

class Game : public octo::DefaultKeyboardListener, public IContactListener
{
public:
	Game();

	void			setup();
	void			loadLevel(std::string const& fileName);

	void			update(sf::Time frameTime);
	void			draw(sf::RenderTarget& render, sf::RenderStates states)const;
private:
	void			followPlayer();
private:
	PhysicsEngine &		m_engine;
	GameClock			m_gameClock;
	BiomeManager		m_biomeManager;
	SkyManager			m_skyManager;
	GroundManager		m_groundManager;
	ParallaxScrolling	m_parallaxScrolling;
	CharacterOcto		m_octo;
	CharacterNpc		m_npc;
	CharacterBasicNpc	m_bnpc;

	bool onPressed(sf::Event::KeyEvent const & event);
	void onShapeCollision(AShape * shapeA, AShape * shapeB);

};

#endif
