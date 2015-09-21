#ifndef AGAMEOBJECT_HPP
# define AGAMEOBJECT_HPP
# include <type_traits>

enum class GameObjectType
{
	Player = 1u,
	PlayerEvent = 2u,
	Tile = 4u,
	Decor = 8u,
	Portal = 16u,
	Elevator = 32u,
	CedricNpc = 64u,
	SpaceShip = 128u,
	RepairNanoRobot = 256u,
	JumpNanoRobot = 512u,
	DoubleJumpNanoRobot = 1024u,
	SlowFallNanoRobot = 2048u,
	GroundTransformNanoRobot = 4096u,
	FranfranNpc = 8192u,
	JuNpc = 16384u,
	OldDesertStaticNpc = 32768u,
	Bouibouik = 65536u,
	GuiNpc = 131072u,
	FannyNpc = 262144,
	TurbanNpc = 524288,
	Tent = 129u,
	Firecamp = 130u,
	Water = 131u
};

/*!
 *	\class AGameObject
 *	\brief Base of all game object
 *
 *	You can use gameObjectCast() instead dynamic_cast. 
 *	\code
 *	class Test0 : public AGameObject<GameObjectType::Player>
 *	{
 *	};
 *	
 *	class Test1 : public AGameObject<GameObjectType::Npc>
 *	{
 *	};
 *	
 *	AGameObjectBase*	o0 = new Test0;
 *	AGameObjectBase*	o1 = new Test1;
 *	Test0*				result0 = gameObjectCast<Test0>(o0);
 *	Test1*				result1 = gameObjectCast<Test1>(o1);
 *	Test0*				result2 = gameObjectCast<Test0>(o1);
 *	Test1*				result3 = gameObjectCast<Test1>(o0);
 *	assert (result0 != nullptr);
 *	assert (result1 != nullptr);
 *	assert (result2 == nullptr);
 *	assert (result3 == nullptr);
 *	\endcode
 */
class AGameObjectBase
{
public:
	virtual					~AGameObjectBase() = 0;
	virtual GameObjectType	getObjectType()const = 0;
};

template <GameObjectType Type>
class AGameObject : public AGameObjectBase
{
public:
	static constexpr GameObjectType const	ObjectType = Type;

	virtual inline GameObjectType	getObjectType()const
	{
		return (Type);
	}
};

template <class T, class O>
static T*	gameObjectCast(O* object)
{
	static_assert( std::is_base_of<AGameObjectBase, T>::value, " T must be derived from AGameObject" );
	static_assert( std::is_base_of<AGameObjectBase, O>::value, " O must be derived from AGameObject" );
	
	T*	result = nullptr;

	if (T::ObjectType == object->getObjectType())
	{
		result = static_cast<T*>(object);
	}
	return (result);
}

#endif
