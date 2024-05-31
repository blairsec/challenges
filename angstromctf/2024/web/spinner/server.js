const fs = require('fs')
const http = require('http')

const index = fs.readFileSync('./index.html')
const server = http.createServer((req, res) => {
    if (req.url === '/') {
        res.writeHead(200, { 'Content-Type': 'text/html' })
        res.end(index)
    } else if (req.url === '/falg' && req.method === 'POST') {
        res.writeHead(200, { 'Content-Type': 'text/plain' })
        res.end(process.env.FLAG ?? 'missing flag')
    } else {
        res.writeHead(404, { 'Content-Type': 'text/plain' })
        res.end('not found')
    }
})

server.listen(3000)
