def encode(x):
    import random
    words = [(['A'],['The']),
            (['robin','eagle','falcon','hawk','bluejay'],['sheep','goat','cow','pig','horse']),
            (['was'],['is']),
            (['a'],['the']),
            (['big','small','tiny','huge','medium-sized'],['fast','slow','speedy','quick','sluggish']),
            (['robin','eagle','falcon','hawk','bluejay'],['sheep','goat','cow','pig','horse']),
            (['actor','dancer','singer','speaker'],['pitcher','hitter','catcher','runner'])]

    end = []
    useable = [bin(ord(i))[2:].zfill(7) for i in x]
    print(useable)
    for char in useable:
        currEnc = []
        for i in range(7):
            poss = words[i][int(char[i])]
            currEnc.append(poss[random.randint(0,len(poss)-1)])
        end.append(' '.join(currEnc)+'.')
    return end
            
