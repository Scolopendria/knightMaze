# Start
data = []

while 1:
	dataPoint = input().split()
	if len(dataPoint) == 0:
		break
	data.append([int(dataPoint[0]), int(dataPoint[1])])

print(f"{len(data)} data points received.\n")

adjustedData = []
previousTime = float(0.0)
previousSolutionCount = 0

for dataPoint in data:
    time = float(dataPoint[1])
    if time > 4000:
        time =  (time - 50)/ 1000000
    adjustedData.append([dataPoint[0], dataPoint[0]-previousSolutionCount, time-previousTime])
    previousSolutionCount = dataPoint[0]
    previousTime = time

solRate = []

for adjDataPoint in adjustedData:
    solRate.append([adjDataPoint[0], float(adjDataPoint[2])/float(adjDataPoint[1])])

print("Start")
for entry in solRate:
	print(f"{entry[0]}	{entry[1]:.4e}")
print("End")