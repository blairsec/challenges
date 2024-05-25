import random,string

def getKeystream(numChars):
    cards = list(range(1,55))
    random.shuffle(cards)
    print(cards)

    keystream = []

    num = 0
    while num < numChars:
        for i in range(len(cards)):
            if cards[i] == 53:
                Ajoker = i
                break
        cards.pop(Ajoker)
        if Ajoker == 53:
            cards.insert(1,53)
            Ajoker = 1
        else:
            cards.insert(Ajoker+1,53)
            Ajoker += 1

        for i in range(len(cards)):
            if cards[i] == 54:
                Bjoker = i
                break
        cards.pop(Bjoker)
        if Bjoker >= 52:
            cards.insert((Bjoker+2)%53,54)
            Bjoker = (Bjoker+2)%53
        else:
            cards.insert(Bjoker+2,54)
            Bjoker += 2

        for i in range(len(cards)):
            if cards[i] == 53:
                Ajoker = i
                break
        
        if Ajoker < Bjoker:
            cards = cards[Bjoker+1:]+cards[Ajoker:Bjoker+1]+cards[:Ajoker]
        elif Ajoker > Bjoker:
            cards = cards[Ajoker+1:]+cards[Bjoker:Ajoker+1]+cards[:Bjoker]
        else:
            print(cards)
            return
        
        if cards[-1] == 54:
            count = 53
        else:
            count = cards[-1]
        cards = cards[count:-1] + cards[:count] + cards[-1:]

        if cards[0] == 54:
            card = cards[53]
        else:
            card = cards[cards[0]]
        if card == 53 or card == 54:
            continue
        
        if card >= 26:
            card -= 26
        keystream.append(card)
        num+=1
    return keystream


def encrypt(msg):
    if not all([i in string.ascii_uppercase for i in msg]): return

    key = getKeystream(len(msg))
    ct = ""
    for i in range(len(msg)):
        ct += chr(((ord(msg[i])-65+key[i])%26)+65)

    return (ct,key)

def decrypt(ct, key):
    pt = ""
    for i in range(len(ct)):
        pt += chr(((ord(ct[i])-65-key[i])%26)+65)

    return pt
