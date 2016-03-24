import fileinput
import re
import sys

# This script write several file to add a npc in instance
# /!\ It doesn't create the npc class
# Need to be run from octogame/
# Need to add text in dialogs.txt

if len(sys.argv) != 3:
  print "Use : python add_pnj_instance.py <ClassName> <SPRITENAME_OSS>"
  exit();

className = sys.argv[1]
spriteName = sys.argv[2]

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

saveLine = ""
for line in fileinput.input('GameObjects/AGameObject.hpp', inplace=1):
    if line.startswith('//Script AddNpc'):
        index = get_num(saveLine) + 1
        print "\t" + className + " = " + str(index) + "u,\n",
        print line
    else:
        saveLine = line
        print line,

for line in fileinput.input('Makefile', inplace=1):
    if line.startswith('#Script AddNpc'):
        print "\t\t\t\tNpcs/" + className + ".cpp\t\t\t\t\\",
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
    else:
        print line,


