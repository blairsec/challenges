from Crypto.Util.number import getPrime
from secrets import randbelow
from primefac import factorint,_primefac
from math import gcd
lcm = lambda a,b:(a*b)//gcd(a,b)
print("We define a^^b to be such that a^^0 = 1 and a^^b = a^(a^^(b-1)), where x^y represents x to the power of y.")
print("Given this, find a positive integer x such that a^^x = b mod p.")
nch = 10
for i in range(nch):
	n = getPrime(70+11*i)
	print(f"Generating challenge {i+1} of {nch}...")
	p = n
	a = randbelow(p-2)+2
	phi = [p]
	while p>1:
		f = factorint(p, methods=(_primefac.pollardRho_brent, _primefac.pollard_pm1, _primefac.williams_pp1, _primefac.ecm, _primefac.mpqs, _primefac.fermat))
		p = 1
		for i in f:
			if i!=2 or f[i]<=2:
				p = lcm(p,(i-1)*pow(i,f[i]-1))
			else:
				p = lcm(p,pow(i,f[i]-2))
		phi.append(p)
	def tetmod(a,e,n):
		cur = 1
		e = min(e,len(phi)-1)
		while e>0:
			e-=1
			cur = pow(a,cur,phi[e])
		return cur
	e = randbelow(len(phi))+1
	b = tetmod(a,e,n)
	print("p =",n)
	print("a =",a)
	print("b =",b)
	try:
		x = int(input("Enter x: "))
		assert(x>0)
		if tetmod(a,x,n)==b:
			print("Correct!")
		else:
			print("Wrong!")
			break
	except:
		print("bad input")
		break
else:
	print("flag: actf{lets_stick_to_discrete_log_for_now...}")