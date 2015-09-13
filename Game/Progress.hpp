#ifndef PROGRESS_HPP
# define PROGRESS_HPP
# include <SFML/System/Vector2.hpp>
# include <string>
# include <memory>

class CharacterOcto;
class Progress
{
public:
	static Progress & getInstance(void);
	void	load(std::string const & filename);
	void	save();

	void			setCharacterOcto(CharacterOcto * octo);
	inline void		setOctoPos(sf::Vector2f position){ m_data.octoPos = position; }
	sf::Vector2f	getOctoPos();

	inline void		setCanUseAction(bool action){ m_action = action; }
	inline void		setCanWalk(bool walk){ m_walk = walk; }
	inline void		setCanJump(bool jump){ m_jump = jump; }
	inline void		setCanDoubleJump(bool doubleJump){ m_doubleJump = doubleJump; }
	inline void		setCanSlowFall(bool slowFall){ m_slowFall = slowFall; }
	inline void		setCanUseElevator(bool Elevator){ m_elevator = Elevator; }

	bool			canUseAction();
	bool			canWalk();
	bool			canJump();
	bool			canDoubleJump();
	bool			canSlowFall();
	bool			canUseElevator();

private:
	Progress();
	struct data{
		data() :
			nb_nanoRobot(0u)
		{
		}
		data(std::size_t nb_nanoRobot, sf::Vector2f position) :
			nb_nanoRobot(nb_nanoRobot),
			octoPos(position)
			//TODO add elevator
		{
		}

		std::size_t		nb_nanoRobot;
		sf::Vector2f	octoPos;
	};
	static std::unique_ptr<Progress>		m_instance;
	CharacterOcto *							m_octo;
	std::string								m_filename;
	data									m_data;
	bool									m_action;
	bool									m_walk;
	bool									m_jump;
	bool									m_doubleJump;
	bool									m_slowFall;
	bool									m_elevator;

};

#endif
