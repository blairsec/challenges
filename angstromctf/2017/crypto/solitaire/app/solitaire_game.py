#!/usr/bin/python3
rawCards =  [11, 45, 13, 24, 27, 15, 31, 5, 6, 8, 35, 36, 48, 28, 47, 41, 39, 19, 46, 4, 18, 7, 17, 51, 42, 34, 33, 20, 25, 44, 54, 9, 2, 22, 29, 49, 32, 26, 10, 23, 43, 53, 1, 12, 40, 16, 37, 30, 14, 50, 38, 21, 52, 3]
ct = "XZVA{JSJEVJUPWJVNNQWYQYL}"
pt = "actf{wowistheanswerright}"

def getCardScoreName(num):
    if num == 54:
        return (13, "Joker B")
    if num == 53:
        return (13, "Joker A")
    suit = (num-1)//13
    score = num%13
    if suit == 0:
        suitName = "Clubs"
    elif suit == 1:
        suitName = "Diamonds"
    elif suit == 2:
        suitName = "Hearts"
    else:
        suitName = "Spades"
    if score == 0:
        return (11, "King of " + suitName)
    if score == 1:
        return (12, "Ace of " + suitName)
    if score == 12:
        return (10, "Queen of " + suitName)
    if score == 11:
        return (9, "Jack of " + suitName)
    return (score-2,str(score)+" of "+suitName)


cards = [getCardScoreName(i) for i in rawCards]

hand1 = [cards[i-1] for i in range(1,55,2)]
hand2 = [cards[i] for i in range(1,55,2)]

print("Welcome to the Game of War! If you win you get the flag!")
input("Are you ready to play? (Hit Enter to Continue) ")

while len(hand1) > 0 and len(hand2) > 0:
    print()
    print("You have",len(hand2),"cards and your opponent has",len(hand1),"cards")
    print()
    card1 = hand1.pop(0)
    card2 = hand2.pop(0)
    print("Your opponent's card was:",card1[1])
    input("Press enter to flip your card ")
    print("Your card was:",card2[1])
    if card1[0] > card2[0]:
        print("Your opponent won!")
        hand1.append(card1)
        hand1.append(card2)
    elif card1[0] < card2[0]:
        print("You won!")
        hand2.append(card1)
        hand2.append(card2)
    else:
        print("War!")
        war = [card1,card2]
        while len(hand1) > 1 and len(hand2) > 1 and card1[0] == card2[0]:
            war.append(hand1.pop(0))
            war.append(hand2.pop(0))
            card1 = hand1.pop(0)
            card2 = hand2.pop(0)
            print("Your opponent flipped flipped a(n)",war[-2][1],"and a(n)",card1[1])
            input("Press enter to flip your cards")
            print("You flipped  a(n)",war[-1][1],"and a(n)",card1[1])
        if card1[0] == card2[0]:
            hand1.pop(0)
            hand2.pop(0)
            break
        elif card1[0] > card2[0]:
            print("Your opponent won the battle!")
            for i in war: hand1.append(i)
            hand1.append(card1)
            hand1.append(card2)
        else:
            print("You won the battle!")
            for i in war: hand2.append(i)
            hand2.append(card1)
            hand2.append(card2)

if len(hand1) == 0:
    print("You won the game! Here is the unencrypted flag",pt)

else:
    print("You lose! Just for lolz here is an encrypted flag for you to enjoy:",ct)
        

