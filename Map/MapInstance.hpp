#ifndef MAPINSTANCE_HPP
# define MAPINSTANCE_HPP

# include <SFML/Graphics.hpp>
# include <Array3D.hpp>
# include <AudioManager.hpp>
# include "IMapTransformable.hpp"
# include "Tile.hpp"

namespace octo
{
	class LevelMap;
}

class MapInstance : public IMapTransformable
{
public:
	MapInstance(std::size_t position, std::string const & resourceId);
	virtual ~MapInstance(void) = default;

	void playSound(void);
	void swapDepth(void);
	void registerDepth(void);
	void nextStep(void);
	void previousStep(void);
	inline Tile const & get(std::size_t x, std::size_t y) const { return m_tiles(x, y, m_depth); }
	inline std::size_t getWidth(void) const { return m_tiles.columns(); }
	inline std::size_t getHeight(void) const { return m_tiles.rows(); }
	inline sf::IntRect const & getCornerPositions(void) const { return m_cornerPositions; }
	inline std::size_t getDepth(void) { return static_cast<std::size_t>(m_depth); }
	inline std::size_t getMaxDepth(void) { return m_tiles.depth(); }

	static void setTransitionType(Tile & tile);

private:
	MapInstance(void) = delete;
	void setStartTransition(int transitionType, Tile & tile, int x, int y);

	octo::Array3D<Tile>				m_tiles;
	octo::LevelMap const &			m_levelMap;
	sf::IntRect						m_cornerPositions;
	int								m_depth;
	int								m_oldDepth;
	octo::AudioManager::SoundPtr	m_soundPtr;
};

#endif
