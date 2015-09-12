#ifndef PROGRESS_HPP
# define PROGRESS_HPP
# include <SFML/System/Vector2.hpp>
# include <string>
# include <memory>

class Progress
{
public:
	static Progress & getInstance(void);
	void	load(std::string filename);
	void	save();

	inline void	setCanUseAction(bool action){ m_data.action = action; }
	inline void	setCanWalk(bool walk){ m_data.walk = walk; }
	inline void	setCanJump(bool jump){ m_data.jump = jump; }
	inline void	setCanDoubleJump(bool doubleJump){ m_data.doubleJump = doubleJump; }
	inline void	setCanSlowFall(bool slowFall){ m_data.slowFall = slowFall; }
	inline void	setCanUseElevator(bool Elevator){ m_data.elevator = Elevator; }

	bool	canUseAction();
	bool	canWalk();
	bool	canJump();
	bool	canDoubleJump();
	bool	canSlowFall();
	bool	canUseElevator();

private:
	Progress();
	struct data{
		data() :
			nb_particles(0u),
			action(false),
			walk(false),
			jump(false),
			doubleJump(false),
			slowFall(false),
			elevator(false)
		{}

		data(std::size_t nb_particles, bool action, bool walk, bool jump, bool doubleJump, bool slowFall, bool elevator) :
			nb_particles(nb_particles),
			action(action),
			walk(walk),
			jump(jump),
			doubleJump(doubleJump),
			slowFall(slowFall),
			elevator(elevator)
		{}

		std::size_t	nb_particles;
		bool		action;
		bool		walk;
		bool		jump;
		bool		doubleJump;
		bool		slowFall;
		bool		elevator;
		sf::Vector2f	position;
	};
	static std::unique_ptr<Progress>		m_instance;
	std::string		m_filename;
	data			m_data;
};

#endif
