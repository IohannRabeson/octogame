import fileinput
import re
import sys
import shutil

# This script write several file to add a npc in instance
# /!\ It doesn't create the npc class
# Need to be run from octogame/
# Need to add text in dialogs.txt

if len(sys.argv) != 3:
  print "Use : python add_npc.py <ClassName> <SPRITENAME_OSS>"
  exit();

className = sys.argv[1]
spriteName = sys.argv[2]

shutil.copy2('Npcs/PeaNpc.cpp', 'Npcs/' + className + ".cpp")
for line in fileinput.input('Npcs/' + className + '.cpp', inplace=1):
    line = line.replace('PeaNpc', className)
    print (line.replace('PEA_OSS', spriteName)),

shutil.copy2('Npcs/PeaNpc.hpp', 'Npcs/' + className + ".hpp")
for line in fileinput.input('Npcs/' + className + '.hpp', inplace=1):
    line = line.replace('PeaNpc', className)
    print (line.replace('PEA_HPP', className.upper() + '_HPP')),

for line in fileinput.input('Game/CharacterOcto.cpp', inplace=1):
    if line.startswith('//Script AddNpc'):
        print line,
        print "\t\t| static_cast<std::size_t>(GameObjectType::" + className + ")"
    else:
        print line,

for line in fileinput.input('Game/Game.cpp', inplace=1):
    if line.startswith('//Script AddNpc Include'):
        print line,
        print "#include \"" + className + ".hpp\""
    elif line.startswith('//Script AddNpc GameObject'):
        print line,
        print "\t\tcase GameObjectType::" + className + ":\n",
        print "\t\t\tgameObjectCast<" + className + ">(gameObject)->collideOctoEvent(octo);\n",
        print "\t\t\tbreak;"
    else:
        print line,


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


