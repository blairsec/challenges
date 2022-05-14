import requests

s = ""

while True:
    print(s)
    p = requests.get("http://localhost:8080/" + s)
    if "actf" in p.text:
        print(p.text)
        break
    s = p.text.strip().split()[-1]
