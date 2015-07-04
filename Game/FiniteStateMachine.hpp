/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FiniteStateMachine.hpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irabeson <irabeson@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/07/03 14:24:18 by irabeson          #+#    #+#             */
/*   Updated: 2015/07/03 19:51:22 by irabeson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FINITESTATEMACHINE_HPP
# define FINITESTATEMACHINE_HPP
# include <memory>
# include <limits>
# include <map>
# include <functional>
# include <cstdint>
# include <set>
# include <ostream>

# include <SFML/System/Time.hpp>

/*!
 *	\class FiniteStateMachine
 *	\brief Provide an automaton to defines complex system behavior
 *
 */
class FiniteStateMachine
{
	class							Transition;
public:
	/*!	Event identifier */
	typedef std::uint64_t			EventId;

	class							AState;

	/*!	State pointer */
	typedef std::shared_ptr<AState>	StatePtr;

	/*!	Type of the function called when a transition is fired */
	typedef std::function<void()>	TransitionCallback;

	/*!	Null event idenitifier */
	static constexpr EventId const	NullEvent = std::numeric_limits<EventId>::max();


	/*!	Create an empty state machine */
	FiniteStateMachine();

	/*!	Get the current state or null if the machine is stopped */
	StatePtr const&	getCurrentState()const;

	/*!	Get the start state */
	StatePtr const& getStartState()const;

	/*!	Add a transition between 2 nodes
	 *
	 *	If \a source or \a target are not already in the machine's graph there
	 *	are added.<br>
	 *	If this transition is already set, the old transition is replaced.
	 */
	void		addTransition(EventId eventId, StatePtr source, StatePtr target, TransitionCallback callback = TransitionCallback());

	/*!	Define the event that will be triggered by next call to update() */
	void		setNextEvent(EventId eventId);

	/*!	Update the automaton
	 *
	 *	If a change has been programmed then is performed.<br>
	 *	Call update method of the current state.
	 */
	void		update(sf::Time frameTime);

	/*!	Define the start state */
	void		setStart(StatePtr const& state);

	/*!	Start automaton 
	 *	
	 *	This method requiere a non null start state.
	 *	\see setStart()
	 */
	void		start();
	
	/*!	Checks if the machine has a start state */
	bool		isReady()const;

	/*!	Export the machine states graph as dot langage */
	void		exportDot(std::ostream& os, std::string const& name);
private:
	StatePtr			m_current;
	EventId				m_nextEventId;
	std::set<StatePtr>	m_states;
	StatePtr			m_startState;
	bool				m_isRunning;
};

/*!	Abstract state
 *
 *	Each state can define his own behavior and have a list of possible transition.<br>
 *	A transition is mapped to an event id.<br>
 *
 *	\note A state is in fact a node, and transitions are edges of the automaton graph.
 */
class FiniteStateMachine::AState
{
	typedef std::map<EventId, Transition>	Transitions;
public:
	typedef typename Transitions::const_iterator	ConstIterator;

	explicit AState(std::string const& name);
	virtual ~AState();

	virtual void		start() = 0;
	virtual void		stop() = 0;
	virtual void		update(sf::Time frameTime) = 0;

	void				addTransition(EventId eventId, StatePtr target);
	void				addTransition(EventId eventId, StatePtr target, TransitionCallback callback);
	StatePtr			getNext(EventId eventId);
	ConstIterator		begin()const;
	ConstIterator		end()const;
	std::string const&	getName()const;
private:
	Transitions		m_transitions;
	std::string		m_name;
};

/*!	Transition between two states
 *	
 *	When a transition is triggered a callback function is called.
 */
class FiniteStateMachine::Transition
{
public:
	explicit Transition(EventId triggerEventId = NullEvent);
	explicit Transition(EventId triggerEventId, StatePtr target, TransitionCallback callback = TransitionCallback());

	void			notifyCallback();
	StatePtr const&	getTargetNode()const;
	EventId			getTriggerEvent()const;
private:
	EventId				m_eventId;
	TransitionCallback	m_callback;
	StatePtr			m_target;
};

#endif
