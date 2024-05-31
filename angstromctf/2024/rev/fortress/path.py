# i wrote this right after fortress.c so i am incredibly imperative brained
# forgive the awful code

from typing import Optional, List

path = [
    '          ',
    ' >v>>v>>v ',
    ' ^>^v<^<v ',
    ' ^v<v>>^v ',
    '>^v^<^<v< ',
    ' v<>>v^>v ',
    ' v>^v<^<v ',
    ' v^<>>>^v ',
    ' >>^v<<<< ',
    '    v     ',
]

def to_delta(direction):
    if direction == '>':
        return (0, 1)
    if direction == '<':
        return (0, -1)
    if direction == '^':
        return (-1, 0)
    if direction == 'v':
        return (1, 0)
    raise ValueError(f'Invalid direction: {direction}')

def from_direction(direction):
    if direction == (0, 1):
        return '>'
    if direction == (0, -1):
        return '<'
    if direction == (-1, 0):
        return '^'
    if direction == (1, 0):
        return 'v'
    raise ValueError(f'Invalid direction: {direction}')

entries: List[List[Optional[int]]] = [
    [None] * 8
    for _ in range(8)
]

options = [
    ((0, -1), (0, 1)),
    ((-1, 0), (1, 0)),
    ((-1, 0), (0, 1)),
    ((-1, 0), (0, -1)),
    ((0, -1), (1, 0)),
    ((1, 0), (0, 1)),
]

for i in range(8):
    for j in range(8):
        out = path[i + 1][j + 1]
        into = None
        for dx, dy in [(0, 1), (0, -1), (1, 0), (-1, 0)]:
            adjacent_x = i + dx
            adjacent_y = j + dy
            symbol = path[adjacent_x + 1][adjacent_y + 1]

            if symbol == ' ': continue

            move_x, move_y = to_delta(symbol)
            if (dx, dy) == (-move_x, -move_y):
                into = (dx, dy)
                break
        if into is None: raise ValueError('Sum ting wong')

        try_a = (into, to_delta(out))
        try_b = (to_delta(out), into)
        if try_a in options:
            entries[i][j] = options.index(try_a)
        elif try_b in options:
            entries[i][j] = options.index(try_b)
        else:
            raise ValueError('Sum ting wong')

flat = [item for sublist in entries for item in sublist]
pairs = [(flat[i], flat[i + 1]) for i in range(0, len(flat), 2)]
key = "abcdefghijklmnopqrstuvwxyz123456789_"

ans = []
for a, b in pairs:
    if a is None or b is None: raise ValueError('Sum ting wong')
    num = a * 6 + b
    ans.append(num)

flag = '1m_re4l1ly_hop1ng_7hi5_i5_un1qu3'
decoded = [key.index(c) for c in flag]

shifts = [(a - d) % 36 for a, d in zip(ans, decoded)]
print(decoded)
print(ans)
print(shifts)
