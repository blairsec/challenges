import string

rotors = [['s', 'y', 'd', 'o', 'q', 'f', 'h', 'p', 'n', 'w', 'l', 't', 'b', 'm', 'x', 'a', 'u', 'z', 'e', 'c', 'k', 'i', 'r', 'v', 'j', 'g'],
          ['i', 'w', 'n', 'l', 's', 'c', 'x', 'h', 'm', 'b', 'g', 'd', 'e', 't', 'u', 'f', 'v', 'q', 'y', 'z', 'j', 'p', 'o', 'k', 'a', 'r'],
          ['x', 'd', 'o', 'n', 'y', 'u', 'c', 'z', 'f', 'p', 'a', 'v', 'g', 'j', 'm', 'q', 's', 't', 'e', 'h', 'r', 'w', 'i', 'k', 'b', 'l'],
          ['o', 'n', 'c', 'v', 'f', 'p', 't', 'u', 'm', 'g', 'j', 'y', 'l', 'i', 'h', 'x', 's', 'd', 'z', 'a', 'w', 'q', 'r', 'e', 'k', 'b'],
          ['x', 'n', 'm', 'r', 'l', 'k', 'd', 'i', 'h', 'z', 'e', 'g', 's', 'p', 'b', 'j', 'u', 'o', 't', 'y', 'v', 'c', 'q', 'f', 'w', 'a'],
          ['l', 'd', 'e', 's', 'u', 't', 'b', 'a', 'f', 'w', 'k', 'x', 'i', 'y', 'r', 'm', 'n', 'g', 'q', 'h', 'o', 'c', 'v', 'j', 'z', 'p'],
          ['u', 's', 'j', 'l', 'r', 'x', 'o', 'q', 'z', 'e', 'n', 'g', 'm', 'k', 'b', 't', 'd', 'c', 'v', 'f', 'i', 'p', 'a', 'y', 'w', 'h'],
          ['g', 'c', 'l', 'p', 'w', 'i', 'e', 'o', 'r', 'u', 'x', 'y', 'z', 'q', 'n', 'm', 'f', 'b', 't', 'j', 's', 'h', 'a', 'k', 'v', 'd'],
          ['m', 'l', 'x', 'n', 'a', 'r', 'v', 'f', 'c', 'h', 'e', 'd', 's', 'z', 'o', 'k', 'b', 'j', 'y', 'w', 'u', 'i', 't', 'g', 'p', 'q']]

alphabet = {'a':0,'b':1,'c':2,'d':3,'e':4,'f':5,'g':6,'h':7,'i':8,'j':9,'k':10,'l':11,'m':12,'n':13,'o':14,'p':15,'q':16,'r':17,'s':18,'t':19,'u':20,'v':21,'w':22,'x':23,'y':24,'z':25}

key = [18, 11, 21, 1, 0, 4, 21, 12, 19]

def encrypt(text):
    text = text.lower()

    output = ''
    for i in text:
        if i in string.ascii_letters:
            curr = alphabet[i]
            for j in range(len(key)):
                curr = alphabet[rotors[j][(curr+key[j])%26]]
            output += string.ascii_letters[curr]
            increment()
        else:
            output += i
    return output
    
def increment():
    i = len(key)-1
    key[i] += 1
    while i > 0 and key[i] == 26:
        key[i] = 0
        key[i-1] += 1
        i -= 1
    if key[0] == 26:
        key[0] = 0

print(encrypt("Hello world!"))
