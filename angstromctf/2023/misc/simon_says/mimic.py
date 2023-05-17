#!/usr/local/bin/python
import random

animals = ['donkey', 'fish', 'lizard', 'lion', 'dragon', 'dolphin', 'wombat', 'cat', 'dog', 'vulture', 'zebra', 'bear', 'monkey', 'giraffe']
for _ in range(20):
    a1 = random.choice(animals)
    a2 = random.choice(animals)
    print(f"Combine the first 3 letters of {a1} with the last 3 letters of {a2}")
    if input() != a1[:3] + a2[-3:]:
        print("""** (RuntimeError) This is not the animal you're looking for.
    (simon_says 0.1.0) lib/simon_says.ex:28: SimonSays.run_trial/0
    (simon_says 0.1.0) lib/simon_says.ex:15: SimonSays.run/1
    (elixir 1.14.3) lib/kernel/cli.ex:131: anonymous fn/3 in Kernel.CLI.exec_fun/2""")
        break
else:
    with open("flag.txt", "r") as f:
        print(f.read())
