#subKeys = [581,4734,1820,6621,5459,3517]
#emailRaw = "v3rylegitbus1n3ssem41l@gmail.com"
#email = [118, 51, 114, 121, 108, 101, 103, 105, 116, 98, 117, 115, 49, 110, 51, 115, 115, 101, 109, 52, 49, 108, 64, 103, 109, 97, 105, 108, 46, 99, 111, 109]
#nameRaw = "John Jacob Jingleheimer Schmidt "
#name = [74, 111, 104, 110, 32, 74, 97, 99, 111, 98, 32, 74, 105, 110, 103, 108, 101, 104, 101, 105, 109, 101, 114, 32, 83, 99, 104, 109, 105, 100, 116, 32]

#emailRaw = input("Enter email: ")
#nameRaw = input("Enter name: ")
#subKeysRaw = input("Enter key: ").split('-')
emailRaw = "artemis.tosini@example.com"
nameRaw = "Artemis Tosini"
pad= [119,60,30,107,57,19,34,15,36,2,115,89,103,100,33,115,23,30,109,91,4,102,101,81,91,67,87,39,14,106,15,109,47,1,72,68,59,72,94,128,78,31,39,17,51,70,51,74,37,94,51,50,40,96,110,0,6,31,40,103,67,125,87,50]

email = [ord(i) for i in emailRaw]
while len(email) < 32:
    email.append(pad.pop(0))
name = [ord(i) for i in nameRaw]
while len(name) < 32:
    name.append(pad.pop(0))

#subKeys = [int(i) for i in subKeysRaw]

subKeys = [3914,6104,4611,1711,1243,4699]

def swap(arr,a,b):
    temp = arr[a]
    arr[a] = arr[b]
    arr[b] = temp

email = [0x5 ^ i for i in email]
name = [0xf ^ i for i in name]

for i in range(6):
    s1 = sum(email[::(i+2)])
    s2 = sum(email[(i+1)::(i+2)])
    subKeys[i] -= (s1*s2)%(10000)
    subKeys[i] += (sum(name[::2])-sum(name[1::2]))<<2

swap(subKeys,3,4)
swap(subKeys,2,5)
swap(subKeys,1,5)
swap(subKeys,2,3)
swap(subKeys,0,5)
swap(subKeys,4,5)

for i in range(6):
    subKeys[i] += sum(name)
    subKeys[i] += sum(email)

for i in range(6):
    subKeys[i] += sum(email[i*4:i*4+1]) % sum(name[i*4+2:i*4+3])
    


mult = [2,4,6,8,7,5]
ans = [sum(email[:10]),sum(email[10:25]),sum(email[25:]),sum(name[:13]),sum(name[13:20]),sum(name[20:])]
ans = [(mult[i]*ans[i])%10000 for i in range(6)]
print(subKeys)
print(ans)
if subKeys == ans:
    print('true')
else:
    print('false')
