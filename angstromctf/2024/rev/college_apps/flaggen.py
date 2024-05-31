flag = "actf{now_fork_over_your_100k_annual_tuition}"
fillers = ["I","of","it","then","to","who","so","and","if","me","the","which","is","from","that","through"]
buzzwords = ["blockchain", "bigdata", "artificialintelligence", "machinelearning", "cybersecurity", "startup", "research", "passion", "ingenuity", "perseverance", "curiosity", "commitment", "angstromCTF", "community", "underprivileged", "inequity", "inequality", "firstgeneration", "hardship", "contribution", "transformative", "communication", "academics", "curingcancer", "future", "imaginative", "dream", "humanity", "society", "individual", "GPA", "SAT", "ACT", "tomorrow", "MOP", "IMO", "olympiad", "APs", "challenges", "struggles", "rediscovery", "problemsolving", "webframeworks", "chatgpt"]
otherwords = list(map(lambda x: x.strip(), open("regwords.txt").read().replace('\t','').split('\n')[:-1]))
allwords  = buzzwords + otherwords
import random
random.shuffle(allwords)
print(allwords)
print(len(allwords))
goodstring = []
numbers = [None for n in range(500)]
filleri = 0
random.shuffle(buzzwords)
numwords = 0
for i in range(44):
	goodstring.append(buzzwords[i])
	numfills = random.randint(0, 3)
	code = 0xbc000000 | (ord(flag[i]) << 16) | (numwords << 8) | i
	numbers[allwords.index(buzzwords[i])] = code ^ 0x4a72b18c
	for j in range(numfills):
		goodstring.append(fillers[filleri])
		filleri += 1
		filleri %= len(fillers)
	numwords += numfills + 1
for i in range(500):
	if numbers[i] is None:
		while numbers[i] is None or numbers[i] & 0xff000000 == 0xbc000000:
			numbers[i] = random.randint(0, 0x100000000)
outs = ""
for i in range(500):
	outs += allwords[i] + ":" + oct(numbers[i])[2:] + " "
open("admissions-database.txt", "w").write(outs)
print(' '.join(goodstring))
	
