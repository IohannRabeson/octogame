#ifndef STATICTILEOBJECT_HPP
# define STATICTILEOBJECT_HPP

# include <memory>
# include <Array3D.hpp>
# include "IMapTransformable.hpp"

class Tile;

//TODO: rename
class StaticTileObject : public IMapTransformable
{
public:
	StaticTileObject(std::size_t width, std::size_t height, std::size_t depth);
	virtual ~StaticTileObject(void);

	// Load the tilemap from resources
	virtual void load(void);

	void swapDepth(void);
	void registerDepth(void);
	void nextStep(void);
	void previousStep(void);
	Tile const & get(std::size_t x, std::size_t y) const;
	std::size_t getWidth(void) const;
	std::size_t getHeight(void) const;

private:
	StaticTileObject(void) = delete;

	octo::Array3D<Tile *>	m_tiles;
	int						m_depth;
	int						m_oldDepth;

};

#endif
