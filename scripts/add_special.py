import fileinput
import re
import sys

# This script write several file to add a npc in instance
# /!\ It doesn't create the npc class
# Need to be run from octogame/
# Need to add text in dialogs.txt

if len(sys.argv) != 3:
  print "Use : python add_npc.py <ClassName> <SPRITENAME_OSS>"
  exit();

className = sys.argv[1]
spriteName = sys.argv[2]

fhpp = open('Npcs/' + className + '.hpp', 'w')
fhpp.write('#ifndef ' + className.upper() + '_HPP\n\
# define ' + className.upper() + '_HPP\n\
\n\
# include "ASpecialNpc.hpp"\n\
\n\
class ' + className + ' : public ASpecialNpc\n\
{\n\
public:\n\
	' + className + '(void);\n\
	virtual ~' + className + '(void) = default;\n\
\n\
	virtual void setup(void);\n\
\n\
};\n\
\n\
#endif\n')
fhpp.close()

fcpp = open('Npcs/' + className + '.cpp', 'w')
fcpp.write('#include "' + className + '.hpp"\n\
\n\
' + className + '::' + className + '(void) :\n\
	ASpecialNpc(' + spriteName + ')\n\
{\n\
    setType(GameObjectType::' + className + ');\n\
	setSize(sf::Vector2f(70.f, 200.f));\n\
	setOrigin(sf::Vector2f(60.f, 58.f));\n\
	setScale(0.8f);\n\
	setTextOffset(sf::Vector2f(-10.f, -50.f));\n\
	setup();\n\
}\n\
\n\
void ' + className + '::setup(void)\n\
{\n\
	typedef octo::CharacterAnimation::Frame			Frame;\n\
\n\
	getIdleAnimation().setFrames({\n\
			Frame(sf::seconds(0.3f), {0u, sf::FloatRect(), sf::Vector2f()}),\n\
			Frame(sf::seconds(0.3f), {1u, sf::FloatRect(), sf::Vector2f()}),\n\
			Frame(sf::seconds(0.3f), {2u, sf::FloatRect(), sf::Vector2f()}),\n\
			Frame(sf::seconds(0.3f), {3u, sf::FloatRect(), sf::Vector2f()}),\n\
			});\n\
	getIdleAnimation().setLoop(octo::LoopMode::Loop);\n\
\n\
	getSpecial1Animation().setFrames({\n\
			Frame(sf::seconds(0.3f), {4u, sf::FloatRect(), sf::Vector2f()}),\n\
			Frame(sf::seconds(0.3f), {5u, sf::FloatRect(), sf::Vector2f()}),\n\
			Frame(sf::seconds(0.3f), {6u, sf::FloatRect(), sf::Vector2f()}),\n\
			Frame(sf::seconds(0.3f), {7u, sf::FloatRect(), sf::Vector2f()}),\n\
			});\n\
	getSpecial1Animation().setLoop(octo::LoopMode::NoLoop);\n\
\n\
	setupMachine();\n\
}\n')

def get_num(x):
    return int(''.join(ele for ele in x if ele.isdigit() or ele == '.'))

def get_last_num(x):
    return get_num((x.split("+"))[-1]);

saveLine = ""
for line in fileinput.input('GameObjects/AGameObject.hpp', inplace=1):
    if line.startswith('//Script AddNpc'):
        index = get_last_num(saveLine) + 1
        print "\t" + className + "\t\t\t\t= (1 << 15) + " + str(index) + ",\n",
        print line
    else:
        saveLine = line
        print line,

for line in fileinput.input('Makefile', inplace=1):
    if line.startswith('#Script AddNpc'):
        print "\t\t\t\tNpcs/" + className + ".cpp\t\t\t\t\\\n",
        print line
    else:
        saveLine = line
        print line,

for line in fileinput.input('Map/GroundManager.cpp', inplace=1):
    if line.startswith('//Script AddNpc Include'):
        print line,
        print "#include \"" + className + ".hpp\""
    elif line.startswith('//Script AddNpc Factory'):
        print line,
        print "\tm_npcFactory.registerCreator<" + className + ">(" + spriteName + ");"
    elif line.startswith('//Script AddNpc Ground'):
        print line,
        print "\t\t\tcase GameObjectType::" + className + ":\n",
        print "\t\t\t\t{\n",
        print "\t\t\t\t\t" + className + " * npc = new " + className + "();\n",
        print "\t\t\t\t\tnpc->onTheFloor();\n",
        print "\t\t\t\t\tm_npcsOnFloor.emplace_back(gameObject.first, 1, npc);\n",
        print "\t\t\t\t}\n",
        print "\t\t\t\tbreak;"
    else:
        print line,


