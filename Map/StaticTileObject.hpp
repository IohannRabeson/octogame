#ifndef STATICTILEOBJECT_HPP
# define STATICTILEOBJECT_HPP

# include <SFML/Graphics.hpp>
# include <Array2D.hpp>
# include "IMapTransformable.hpp"

class Tile;

class StaticTileObject : public IMapTransformable
{
public:
	StaticTileObject(std::size_t width, std::size_t height, std::size_t depth);
	virtual ~StaticTileObject(void);

	// Load the tilemap from resources
	void load(void);

	void swapDepth(void);
	void registerDepth(void);
	void nextStep(void);
	void previousStep(void);
	inline Tile const & get(std::size_t x, std::size_t y) const { return *m_tiles[m_depth](x, y); }
	inline std::size_t getWidth(void) const { return m_tiles[0].columns(); }
	inline std::size_t getHeight(void) const { return m_tiles[0].rows(); }
	inline sf::IntRect const & getCornerPositions(void) const { return m_cornerPositions; }

private:
	StaticTileObject(void) = delete;

	octo::Array2D<Tile *> *	m_tiles;
	sf::IntRect				m_cornerPositions;
	std::size_t				m_maxDepth;
	int						m_depth;
	int						m_oldDepth;

};

#endif
