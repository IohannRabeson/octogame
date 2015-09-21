#ifndef PROGRESS_HPP
# define PROGRESS_HPP
# include "GroundManager.hpp"
# include <SFML/System/Vector2.hpp>
# include <string>
# include <memory>

class ABiome;
class Progress
{
public:
	static Progress & getInstance(void);

	inline void			addNanoRobot() { m_data.nanoRobotCount++; }
	inline void			removeNanoRobot() { m_data.nanoRobotCount--; }
	inline std::size_t	getNanoRobotCount() { return m_data.nanoRobotCount; }

	void				setNextDestination(Level destination);
	Level				getNextDestination(void) const;

	bool				canMoveMap();
	bool				canRepair();
	bool				canWalk();
	bool				canJump();
	bool				canDoubleJump();
	bool				canSlowFall();
	bool				canUseElevator();
	bool				changeLevel() const;
	void				levelChanged();

	inline void					setOctoPos(sf::Vector2f const & position){ m_octoPos = position; }
	inline sf::Vector2f const&	getOctoPos(){ return m_octoPos; }

	inline void		setReverseSprite(bool reverse){ m_reverseSprite = reverse; }
	inline bool		getReverseSprite(){ return m_reverseSprite; }

	void				load(std::string const & filename);
	void				save();
	void				reset();

private:
	Progress();
	void	init();
	void	saveToFile();
	void	setup();
	struct data{
		data() :
			data(5u, Level::Default,
					5u, 100u,
					true, true)
		{
		}
		data(std::size_t nanoRobot, Level biome,
				std::size_t musicVol, std::size_t soundVol,
				bool fullscreen, bool vsync) :
			nanoRobotCount(nanoRobot),
			destination(biome),
			musicVol(musicVol),
			soundVol(soundVol),
			fullscreen(fullscreen),
			vsync(vsync)
		{
		}
		std::size_t		nanoRobotCount;
		Level			destination;
		std::size_t		musicVol;
		std::size_t		soundVol;
		bool			fullscreen;
		bool			vsync;

	};

	static std::unique_ptr<Progress>		m_instance;
	std::string								m_filename;
	data									m_data;
	bool									m_newSave;
	bool									m_changeLevel;
	bool									m_reverseSprite;
	sf::Vector2f							m_octoPos;
};

#endif
