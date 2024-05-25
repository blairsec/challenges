from flask import Flask, render_template, url_for
from forms import LoginForm

from PIL import Image, ImageDraw
import math,random

app = Flask(__name__)
app.config['WTF_CSRF_ENABLED'] = False

flag = 'actf{comp_abstract_art_2F239B}'

def rotate(point,angle):
	return (point[0]*math.cos(angle)-point[1]*math.sin(angle),point[1]*math.cos(angle)+point[0]*math.sin(angle))

def genImage(location):
    im = Image.open('static/base.png')
    draw = ImageDraw.Draw(im)
    border = 500

    numRedS = random.randint(0,9)
    numGreenS = random.randint(0,9)
    numBlueS = random.randint(0,9)

    numRedT = random.randint(0,9)
    numGreenT = random.randint(0,9)
    numBlueT = random.randint(0,9)

    numRedC = random.randint(0,9)
    numGreenC = random.randint(0,9)
    numBlueC = random.randint(0,9)

    for i in range(numRedS):
        pointX = 10+random.randint(0,border)
        pointY = 10+random.randint(0,border)
        size = random.randint(10,40)
        rotation = random.uniform(0,2*math.pi)
        point = ((pointX,pointY),tuple([sum(x) for x in zip(*[(pointX,pointY),rotate((size,0),rotation)])]),tuple([sum(x) for x in zip(*[(pointX,pointY),rotate((size,size),rotation)])]),tuple([sum(x) for x in zip(*[(pointX,pointY),rotate((0,size),rotation)])]))
        draw.polygon(point,(255-random.randint(0,75),random.randint(0,75),random.randint(0,75)))

    for i in range(numGreenS):
        pointX = 10+random.randint(0,border)
        pointY = 10+random.randint(0,border)
        size = random.randint(10,40)
        rotation = random.uniform(0,2*math.pi)
        point = ((pointX,pointY),tuple([sum(x) for x in zip(*[(pointX,pointY),rotate((size,0),rotation)])]),tuple([sum(x) for x in zip(*[(pointX,pointY),rotate((size,size),rotation)])]),tuple([sum(x) for x in zip(*[(pointX,pointY),rotate((0,size),rotation)])]))
        draw.polygon(point,(random.randint(0,75),255-random.randint(0,75),random.randint(0,75)))

    for i in range(numBlueS):
        pointX = 10+random.randint(0,border)
        pointY = 10+random.randint(0,border)
        size = random.randint(10,40)
        rotation = random.uniform(0,2*math.pi)
        point = ((pointX,pointY),tuple([sum(x) for x in zip(*[(pointX,pointY),rotate((size,0),rotation)])]),tuple([sum(x) for x in zip(*[(pointX,pointY),rotate((size,size),rotation)])]),tuple([sum(x) for x in zip(*[(pointX,pointY),rotate((0,size),rotation)])]))
        draw.polygon(point,(random.randint(0,75),random.randint(0,75),255-random.randint(0,75)))

    for i in range(numRedT):
        pointX = 10+random.randint(0,border)
        pointY = 10+random.randint(0,border)
        length1 = random.randint(10,40)
        length2 = random.randint(0,length1)
        height = random.randint(10,40)
        rotation = random.uniform(0,2*math.pi)
        point = ((pointX,pointY),tuple([sum(x) for x in zip(*[(pointX,pointY),rotate((length1,0),rotation)])]),tuple([sum(x) for x in zip(*[(pointX,pointY),rotate((length1,height),rotation)])]))
        draw.polygon(point,(255-random.randint(0,75),random.randint(0,75),random.randint(0,75)))

    for i in range(numGreenT):
        pointX = 10+random.randint(0,border)
        pointY = 10+random.randint(0,border)
        length1 = random.randint(10,40)
        length2 = random.randint(0,length1)
        height = random.randint(10,40)
        rotation = random.uniform(0,2*math.pi)
        point = ((pointX,pointY),tuple([sum(x) for x in zip(*[(pointX,pointY),rotate((length1,0),rotation)])]),tuple([sum(x) for x in zip(*[(pointX,pointY),rotate((length1,height),rotation)])]))
        draw.polygon(point,(random.randint(0,75),255-random.randint(0,75),random.randint(0,75)))

    for i in range(numBlueT):
        pointX = 10+random.randint(0,border)
        pointY = 10+random.randint(0,border)
        length1 = random.randint(10,40)
        length2 = random.randint(0,length1)
        height = random.randint(10,40)
        rotation = random.uniform(0,2*math.pi)
        point = ((pointX,pointY),tuple([sum(x) for x in zip(*[(pointX,pointY),rotate((length1,0),rotation)])]),tuple([sum(x) for x in zip(*[(pointX,pointY),rotate((length1,height),rotation)])]))
        draw.polygon(point,(random.randint(0,75),random.randint(0,75),255-random.randint(0,75)))

    for i in range(numRedC):
        pointX = 10+random.randint(0,border)
        pointY = 10+random.randint(0,border)
        size = random.randint(10,40)
        point = ((pointX,pointY),(pointX+size,pointY+size))
        draw.ellipse(point,(255-random.randint(0,75),random.randint(0,75),random.randint(0,75)))

    for i in range(numGreenC):
        pointX = 10+random.randint(0,border)
        pointY = 10+random.randint(0,border)
        size = random.randint(10,40)
        point = ((pointX,pointY),(pointX+size,pointY+size))
        draw.ellipse(point,(random.randint(0,75),255-random.randint(0,75),random.randint(0,75)))

    for i in range(numBlueC):
        pointX = 10+random.randint(0,border)
        pointY = 10+random.randint(0,border)
        size = random.randint(10,40)
        point = ((pointX,pointY),(pointX+size,pointY+size))
        draw.ellipse(point,(random.randint(0,75),random.randint(0,75),255-random.randint(0,75)))

    im.save('static/'+location,'PNG')
    numbers = [numRedS,numRedT,numRedC,
               numBlueS,numBlueT,numBlueC,
               numGreenS,numGreenT,numGreenC]
    choice = random.randint(0,len(numbers)-1)
    return (numbers,choice)


numbers,choice = genImage('captcha.png')
filename = 'captcha.png'


@app.route('/', methods=['GET', 'POST'])

def main():
    global numbers,choice,filename
    informationPrompt = ['red squares','red triangle','red circles',
                   'blue squares','blue triangles','blue circles',
                   'green squares','green triangles','green circles']
    form = LoginForm()
    if form.validate_on_submit():
        if form.question1.data == numbers[choice]:
            numbers,choice = genImage(filename)
            if form.pin.data == 612:
                return render_template('index.html',
                                       flag=flag,
                                       words=informationPrompt[choice],
                                       form=form,
                                       picture=url_for('static',filename=filename))
            else:
                return render_template('index.html',
                                       flag='Wrong pin!',
                                       words=informationPrompt[choice],
                                       form=form,
                                       picture=url_for('static',filename=filename))
        else:
            return render_template('index.html',
                                   flag='Bad captcha!',
                                   words=informationPrompt[choice],
                                   form=form,
                                   picture=url_for('static',filename=filename))
    return render_template('index.html',
                           flag='',
                           words=informationPrompt[choice],
                           form=form,
                           picture=url_for('static',filename=filename))

@app.after_request
def add_header(response):
    response.headers['Cache-Control'] = 'no-store'
    return response

if __name__ == '__main__':
    app.run()
