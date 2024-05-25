from flask_wtf import Form
from wtforms import IntegerField
from wtforms.validators import DataRequired

class LoginForm(Form):
    question1 = IntegerField('captcha')
    pin = IntegerField('pin_number')
