events = []

events.append("Idle")
events.append("Right")
events.append("Left")
events.append("StartJump")
events.append("DoubleJump")
events.append("Fall")
events.append("DieFall")
events.append("Wait")
events.append("StartSlowFall")
events.append("Death")
events.append("Drink")
events.append("StartElevator")
events.append("Elevator")
events.append("SlowFall1")
events.append("SlowFall2")
events.append("SlowFall3")
events.append("StartWaterJump")
events.append("WaterJump")
events.append("PortalEvent")
events.append("KonamiCode")

result = ""

for event1 in events:
  secondState = "state" + event1
  result += "\n"
  for event2 in events:
    firstState = "state" + event2
    result += "\tmachine.addTransition(" + event1 + ", " + firstState + ", " + secondState + ");\n"

print result
