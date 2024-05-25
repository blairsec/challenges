import random
import string

def genXOR(num):
    f = open('xorFunc.py','w')
    for i in range(num):
        f.write('def xor'+str(i)+'(x):\n')
        nums = [random.randint(0,256) for j in range(32)]
        f.write('\tnums'+str(i+100)+ ' = '+str(nums)+'\n')
        f.write('\treturn [x[i]^nums'+str(i+100)+'[i] for i in range(32)]\n')

    f.close()

def genSub(num):
    f = open('subFunc.py','w')
    for i in range(num):
        f.write('def sub'+str(i)+'(x):\n')
        ints = list(range(0,256))
        random.shuffle(ints)
        f.write('\tsubs'+str(199-i)+ ' = ' + str(ints)+'\n')
        f.write('\treturn [subs'+str(199-i)+'[x[i]] for i in range(32)]\n')
    f.close()

def genSwap(num):
    f = open('swapFunc.py','w')
    for i in range(num):
        f.write('def swap'+str(i)+'(x):\n')
        swaps = list(range(0,32))
        random.shuffle(swaps)
        f.write('\tswaps'+str(300+i*3)+' = ' + str(swaps)+'\n')
        f.write('\treturn [x[swaps[i]] for i in range(32)]\n')
    f.close()
        
        
def genAll(num):
    good = string.ascii_letters+string.digits+'_'
    f = open('allFunc.py','w')
    things = [0]*num+[1]*num+[2]*num
    random.shuffle(things)
    count0=0
    count1=0
    count2=0
    for i in things:
        if i == 0:
            x = string.ascii_letters[random.randint(0,51)]+''.join([good[random.randint(0,len(good)-1)] for i in range(random.randint(5,10))])
            k = string.ascii_letters[random.randint(0,51)]+''.join([good[random.randint(0,len(good)-1)] for i in range(random.randint(5,10))])
            f.write('def xor'+str(count0)+'('+x+'):\n')
            count0+=1
            nums = [random.randint(0,255) for j in range(32)]
            f.write('\tnums'+str(i+100)+ ' = '+str(nums)+'\n')
            f.write('\treturn ['+x+'['+k+']^nums'+str(i+100)+'['+k+'] for '+k+' in range(32)]\n')
        if i == 1:
            x = string.ascii_letters[random.randint(0,51)]+''.join([good[random.randint(0,len(good)-1)] for i in range(random.randint(5,10))])
            k = string.ascii_letters[random.randint(0,51)]+''.join([good[random.randint(0,len(good)-1)] for i in range(random.randint(5,10))])
            f.write('def sub'+str(count1)+'('+x+'):\n')
            count1+=1
            ints = list(range(0,256))
            random.shuffle(ints)
            f.write('\tsubs'+str(199-i)+ ' = ' + str(ints)+'\n')
            f.write('\treturn [subs'+str(199-i)+'['+x+'['+k+']] for '+k+' in range(32)]\n')
        if i == 2:
            x = string.ascii_letters[random.randint(0,51)]+''.join([good[random.randint(0,len(good)-1)] for i in range(random.randint(5,10))])
            k = string.ascii_letters[random.randint(0,51)]+''.join([good[random.randint(0,len(good)-1)] for i in range(random.randint(5,10))])
            f.write('def swap'+str(count2)+'('+x+'):\n')
            count2+=1
            swaps = list(range(0,32))
            random.shuffle(swaps)
            f.write('\tswaps'+str(300+i*3)+' = ' + str(swaps)+'\n')
            f.write('\treturn ['+x+'[swaps'+str(300+i*3)+'['+k+']] for '+k+' in range(32)]\n')
    f.close()

def genFinalFunc(num):
    f = open('form.py','w')
    f.write('x = conAscii(L0oiklh)\n')
    for i in range(num):
        test = random.randint(0,2)
        if test == 0:
            f.write('x=xor'+str(random.randint(0,499))+'(x)\n')
        if test == 1:
            f.write('x=sub'+str(random.randint(0,499))+'(x)\n')
        if test == 2:
            f.write('x=swap'+str(random.randint(0,499))+'(x)\n')
    f.write('conOut(x)\n')
    f.close()
