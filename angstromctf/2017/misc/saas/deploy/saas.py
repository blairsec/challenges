import subprocess, socketserver, signal

PORT = 3072

class incoming(socketserver.BaseRequestHandler):
    def handle(self):
        req = self.request

        def recvline():
            buf = b""
            while not buf.endswith(b"\n") and len(buf) < 64:
                buf = buf + req.recv(1)
            return buf[:-1]
        signal.alarm(120)
        
        req.sendall(b"Welcome to Sed as a Service\n")
        req.sendall(b"Enter your text to find: ")
        src = recvline()
        req.sendall(b"Text to repace with: ")
        dst = recvline()
        req.sendall(b"Source text: ")
        corpus = recvline()
        
        sp = subprocess.Popen(['sed', 's/{}/{}/g'.format(src.decode('utf-8'), dst.decode('utf-8'))], stdin=subprocess.PIPE, stdout=subprocess.PIPE)
        sp.stdin.write(corpus)
        result = sp.communicate()[0]

        req.sendall(result)
        req.close()

class ReusableTCPServer(socketserver.ForkingMixIn, socketserver.TCPServer):
    pass


socketserver.TCPServer.allow_reuse_address = True
server = ReusableTCPServer(("0.0.0.0", PORT), incoming)

print("Server listening on port %d" % PORT)
server.serve_forever()

