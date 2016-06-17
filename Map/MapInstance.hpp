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
	inline Tile const & get(std::size_t x, std::size_t y, std::size_t z) const { return m_tiles(x, y, z); }
	inline float const & getMovementMask(std::size_t x, std::size_t y) const { return m_movementMask(x, y, 0); }
	inline std::size_t getWidth(void) const { return m_tiles.columns(); }
	inline std::size_t getHeight(void) const { return m_tiles.rows(); }
	inline sf::IntRect const & getCornerPositions(void) const { return m_cornerPositions; }
	inline std::size_t getDepth(void) const { return static_cast<std::size_t>(m_depth); }
	inline std::size_t getMaxDepth(void) const { return m_tiles.depth(); }
	inline bool		   isMapHighlight(void) const { return m_isMapHighlight; }

	static void setTransitionType(Tile & tile);

private:
	MapInstance(void) = delete;
	void setStartTransition(int transitionType, Tile & tile, int x, int y);
	void addRadiance(std::size_t depth);

	octo::Array3D<Tile>				m_tiles;
	octo::Array3D<float>			m_movementMask;
	octo::LevelMap const &			m_levelMap;
	sf::IntRect						m_cornerPositions;
	bool							m_isMapHighlight;
	int								m_depth;
	int								m_oldDepth;
	octo::AudioManager::SoundPtr	m_soundPtr;
};

#endif
