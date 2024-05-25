key = b"\x83\x85 [P\xae\xf0\x80\x88\xbf\xf6\xea!\xd7E\xd9\xae\xfc\xfda\xf8$\xb9f\xd6\xbd\xfed\xc5z\\W"

import random

def genPBox():
    output = list(range(16))
    while output == list(range(16)):
        random.shuffle(output)
    return output

def getInvPBox(pBox):
    output = [0 for i in range(16)]
    for i in range(16):
        output[pBox[i]] = i
    return output

def genSBox():
    output = []
    poss = list(range(16))
    while poss == list(range(16)):
        random.shuffle(poss)
    for i in range(4):
        output.append([0 for i in range(4)])
        for j in range(4):
            output[i][j] = poss[i*4+j]
    return output

def getInvSBox(sBox):
    output = [[0 for i in range(4)] for j in range(4)]
    for i in range(4):
        for j in range(4):
            target = bin(sBox[i][j])[2:].zfill(4)
            outTarget = int(target[0] + target[3],2)
            inTarget = int(target[1] + target[2],2)

            outIndex = bin(i)[2:].zfill(2)
            inIndex = bin(j)[2:].zfill(2)
            output[outTarget][inTarget] = int(outIndex[0]+inIndex+outIndex[1],2)
    return output
            

def P(block, permute):
    binary = bin(int.from_bytes(block, byteorder="big"))[2:].zfill(16)
    output = [0 for i in range(16)]
    for i in range(16):
        output[permute[i]] = binary[i]
    return int("".join(output),2).to_bytes(2, byteorder="big")

def S(block, sub):
    blockHex = bytes.hex(block)
    output = ""
    for i in range(4):
        binary = bin(int(blockHex[i],16))[2:].zfill(4)
        outer = int(binary[0] + binary[3],2)
        inner = int(binary[1] + binary[2],2)
        output += hex(sub[i][outer][inner])[2:]
    return bytes.fromhex(output)

def mix(block,key):
    end = b""
    for i in range(len(block)):
        end += (block[i]^key[i]).to_bytes(1, byteorder="big")
    return end

def Eround(block,key,pBox,sBox):
    output = mix(block,key)
    output = S(output,sBox)
    output = P(output,pBox)
    return output

def Dround(block,key,pBox,sBox):
    output = P(block,pBox)
    output = S(output,sBox)
    output = mix(output,key)
    return output

def encrypt(block,keys,rounds,pBox,sBox):
    current = block
    for i in range(rounds-1):
        current = Eround(current,keys[i],pBox,sBox)
    current = mix(current,keys[-1]) #so that last round can"t be reversed

    return current

def decrypt(block,keys,rounds,pBox,sBox):
    current = mix(block,keys[-1])
    for i in range(rounds-2,-1,-1):
        current = Dround(current,keys[i],pBox,sBox)

    return current

def oracle(key, inText, rounds, mode, pBox=None, sBox=None):
    #not a very good padding scheme
    while len(inText) % 4 != 0:
        inText += b"\x00"

    if len(key) != 4*rounds: return False

    keys = [key[i:i+4] for i in range(0,len(key),4)]

    if mode == "encrypt":
        if pBox == None: pBox = genPBox()
        print("The permutation box is: ",pBox)
        if sBox == None:
            sBox = []
            for i in range(4):
                sBox.append(genSBox())
                print("Substitution box ",i, " is: " , sBox[i])
        output = b""
        for i in range(0,len(inText),2):
            output += encrypt(inText[i:i+4],keys,rounds,pBox,sBox)

    elif mode == "decrypt":
        if pBox == None: pBox = genPBox()
        else: pBox = getInvPBox(pBox)
        print("The permutation box is: ",pBox)

        if sBox == None: sBox = [genSBox() for i in range(4)]
        else: sBox = [getInvSBox(i) for i in sBox[::-1]]
        for i in range(4):
                print("Substitution box ",i, " is: " , sBox[i])

        output = b""
        for i in range(0,len(inText),2):
            output += decrypt(inText[i:i+4],keys,rounds,pBox,sBox)
    else: return False

    return bytes.hex(output)

def main():
    while True:
        mode = input("Please enter the mode, encrypt (e) or decrypt (d): ").lower()
        if mode == "e" or mode == "encrypt":
            pt = bytes.fromhex(input("Please enter the hex-encode string you would like to be encrypted: "))
            ct = oracle(key,pt,8,'encrypt')
            print("Here is the hex-encoded cipher text: " + ct)
        if mode == "d" or mode == "decrypt":
            ct = bytes.fromhex(input("Please enter the hex-encoded string you would like to be decrypted: "))
            pBox = input("Please enter the pBox that the encryption used in a comma-separated list: ").split(", ")
            pBox = [int(i) for i in pBox]
            sBox = []
            for i in range(4):
                temp = input("Please enter sBox "+str(i)+" that the encryption used in a comma-separated list: ").split(", ")
                sBox.append([[int(k) for k in temp[j:j+4]] for j in range(0,16,4)])
            pt = oracle(key,ct,8,'decrypt',pBox,sBox)
            print("Here is the hex-encoded plain text: " + pt)

main()
