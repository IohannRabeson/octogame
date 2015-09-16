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

	inline void					setDefaultBiome(std::string name){ m_data.biomeName = name; }
	inline std::string const&	getLevelName(){ return m_data.biomeName; }
	void						setupInfoLevel(ABiome & biome);

	inline void				addNanoRobot(){ m_data.nanoRobotCount++; }
	inline std::size_t		getNanoRobotCount(){ return m_data.nanoRobotCount; }

	inline void				setCanUseAction(bool action){ m_action = action; }
	inline void				setCanWalk(bool walk){ m_walk = walk; }
	inline void				setCanJump(bool jump){ m_jump = jump; }
	inline void				setCanDoubleJump(bool doubleJump){ m_doubleJump = doubleJump; }
	inline void				setCanSlowFall(bool slowFall){ m_slowFall = slowFall; }
	inline void				setCanUseElevator(bool Elevator){ m_elevator = Elevator; }

	bool			canUseAction();
	bool			canWalk();
	bool			canJump();
	bool			canDoubleJump();
	bool			canSlowFall();
	bool			canUseElevator();

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
	data									m_data;
	bool									m_newSave;
	bool									m_action;
	bool									m_walk;
	bool									m_jump;
	bool									m_doubleJump;
	bool									m_slowFall;
	bool									m_elevator;
};

#endif
