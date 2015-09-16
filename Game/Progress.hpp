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
	void	load(std::string const & filename);
	void	save();
	void	reset();

	inline std::string const&	getLevelName(){ return m_data.biomeName; }
	void						setupInfoLevel(ABiome & biome);

	inline void					addNanoRobot(){ m_data.nanoRobotCount++; }
	inline void					removeNanoRobot(){ m_data.nanoRobotCount--; }
	inline std::size_t			getNanoRobotCount(){ return m_data.nanoRobotCount; }

	void						setNextDestination(Level destination);
	Level						getNextDestination(void) const;

	bool			canMoveMap();
	bool			canRepair();
	bool			canWalk();
	bool			canJump();
	bool			canDoubleJump();
	bool			canSlowFall();
	bool			canUseElevator();
	bool			changeLevel() const;
	void			levelChanged();

private:
	Progress();
	void	init();
	void	saveToFile();
	void	setup();
	struct data{
		data() :
			nanoRobotCount(0u),
			biomeName("")
		{
		}
		data(std::size_t nanoRobot, std::string biomeName) :
			nanoRobotCount(nanoRobot),
			biomeName(biomeName)
		{
			//TODO add elevator
		}
		std::size_t		nanoRobotCount;
		std::string		biomeName;
	};
	static std::unique_ptr<Progress>		m_instance;
	std::string								m_filename;
	Level									m_destination;
	data									m_data;
	bool									m_newSave;
	bool									m_changeLevel;

};

#endif
