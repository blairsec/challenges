#!/usr/bin/python3

"""
Understanding any of this code is unnecessary. Just know it follows the rules laid out on the Esolang wiki for interpreting AlphaBeta.
The flag is loaded into memory before the program is executed.
"""

from sys import stdin

num_char = 0
position = 0
new_position = 0
mode = 0
program = [0 for i in range(10000)]
register = [0, 0, 0, [0, 0]]
memory = [0 for i in range(1024)]
temp = 0

def setup():
	global memory
	memory = [97, 99, 116, 102, 123, 101, 115, 111, 108, 97, 110, 103, 115, 95, 115, 117, 114, 101, 95, 97, 114, 101, 95, 102, 117, 110, 33, 125] + [0 for i in range(996)]

def execute_program():
	try:
		global num_char, position, new_position, mode, program, register, memory, temp
		while True:
			new_position = position

			if program[position] == ord('a'):
				register[0] += 1
			elif program[position] == ord('b'):
				register[0] -=1
			elif program[position] == ord('c'):
				register[0] += 10
			elif program[position] == ord('d'):
				register[0] -= 10
			elif program[position] == ord('e'):
				register[0] += 100
			elif program[position] == ord('f'):
				register[0] -= 100
			elif program[position] == ord('g'):
				register[1] += 1
			elif program[position] == ord('h'):
				register[1] -= 1
			elif program[position] == ord('i'):
				register[1] += 10
			elif program[position] == ord('j'):
				register[1] -= 10
			elif program[position] == ord('k'):
				register[1] += 100
			elif program[position] == ord('l'):
				register[1] -= 100
			elif program[position] == ord('m'):
				register[2] = ~register[0]
			elif program[position] == ord('n'):
				register[2] = ~register[1]
			elif program[position] == ord('o'):
				register[2] = register[0] & register[1]
			elif program[position] == ord('p'):
				register[2] = register[0] | register[1]
			elif program[position] == ord('q'):
				register[2] = register[0] ^ register[1]
			elif program[position] == ord('r'):
				register[2] = register[0] + register[1]
			elif program[position] == ord('s'):
				register[2] = register[0] - register[1]
			elif program[position] == ord('t'):
				register[2] = register[0] * register[1]
			elif program[position] == ord('u'):
				register[2] = int(register[0] / register[1])
			elif program[position] == ord('v'):
				register[2] = register[0] % register[1]
			elif program[position] == ord('w'):
				if register[1] >= 0:
					register[2] = int(register[0] ** register[1])
				else:
					raise ValueError
			elif program[position] == ord('x'):
				register[0] = 0
			elif program[position] == ord('y'):
				register[1] = 0
			elif program[position] == ord('z'):
				register[2] = 0
			elif program[position] == ord('A'):
				register[1] = register[0]
			elif program[position] == ord('B'):
				register[0] = register[1]
			elif program[position] == ord('C'):
				register[2] = register[0]
			elif program[position] == ord('D'):
				register[2] = register[1]
			elif program[position] == ord('E'):
				register[0] = register[2]
			elif program[position] == ord('F'):
				register[1] = register[2]
			elif program[position] == ord('G'):
				register[0] = memory[register[3][0]]
			elif program[position] == ord('H'):
				register[1] = memory[register[3][0]]
			elif program[position] == ord('I'):
				memory[register[3][0]] = register[2]
			elif program[position] == ord('J'):
				register[0] = ord(stdin.read(1))
			elif program[position] == ord('K'):
				register[1] = ord(stdin.read(1))
			elif program[position] == ord('L'):
				print(chr(register[2]), end="")
			elif program[position] == ord('M'):
				print(int(register[2]), end="")
			elif program[position] == ord('N'):
				if register[0] == register[1]:
					new_position = register[3][1]
			elif program[position] == ord('O'):
				if register[0] != register[1]:
					new_position = register[3][1]
			elif program[position] == ord('P'):
				if register[0] >= register[1]:
					new_position = register[3][1]
			elif program[position] == ord('Q'):
				if register[0] <= register[1]:
					new_position = register[3][1]
			elif program[position] == ord('R'):
				if register[2] == 0:
					new_position = register[3][1] - 1
			elif program[position] == ord('S'):
				register[3][mode] += 1
			elif program[position] == ord('T'):
				register[3][mode] -= 1
			elif program[position] == ord('U'):
				register[3][mode] += 10
			elif program[position] == ord('V'):
				register[3][mode] -= 10
			elif program[position] == ord('W'):
				register[3][mode] += 100
			elif program[position] == ord('X'):
				register[3][mode] -= 100
			elif program[position] == ord('Y'):
				register[3][mode] = 0
			elif program[position] == ord('Z'):
				mode = (mode + 1) % 2

			if position != new_position:
				position = new_position
			else:
				position += 1

			if position > num_char:
				break
	except:
		print("You broke it. Are you proud?")

setup()
print("Welcome to the AlphaBeta interpreter! The flag is at the start of memory. You get one line:\n> ", end="")
inp = input()
for i in inp:
	program[num_char] = ord(i)
	num_char += 1
execute_program()
