from decimal import Decimal, getcontext
getcontext().prec = 50

# a = (Decimal("137038170799840894036702506618850666645.4935735359"), Decimal("65152771400517747564219320633280207337.919970793738"))
# b = (Decimal("303045314948625741259359768387560215922.9848080391"), Decimal("149741127261784650517551714749427913774.53225517191"))

a = (Decimal("103952305445602140450340939146733637878.377859839"), Decimal("11849775177542605048596462943025342055.03088987913"))
b = (Decimal("7421623705262639381291950098743251863.3264341691"), Decimal("9394938028629474541107190524190692717.59539603520"))

slope = (b[1] - a[1]) / (b[0] - a[0])

midpoint = ((a[0] + b[0]) / 2, (a[1] + b[1]) / 2)

def f(x):
    return slope * (x - midpoint[0]) + midpoint[1]

# start = midpoint[0].to_integral_exact()
start = Decimal(50332264899325901282263460993108212073)

for i in range(0, 20000):
    x = f(i + start)
    diff = abs(x - round(x))
    if diff < Decimal("0.001"):
        print(start + i, x)
