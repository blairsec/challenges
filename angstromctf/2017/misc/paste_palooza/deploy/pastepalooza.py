import subprocess, SocketServer, signal

PORT = 3001

class incoming(SocketServer.BaseRequestHandler):
    def handle(self):
        req = self.request

        def recvline():
            buf = ""
            while not buf.endswith("\n") or len(buf) < 32:
                buf += req.recv(1)
            return buf[:-1]
        signal.alarm(120)
        
        req.sendall("Welcome to paste palooza!\n")
        req.sendall("Currently, only the file access feature is available.\n")
        req.sendall("Access a file by entering its name: ")
        name = recvline()
        result = subprocess.check_output("cd pastes; ./nameaccess "+name+"; exit 0",\
                                         stderr=subprocess.STDOUT,\
                                         shell=True)
        req.sendall(result)
        req.close()

class ReusableTCPServer(SocketServer.ForkingMixIn, SocketServer.TCPServer):
    pass


SocketServer.TCPServer.allow_reuse_address = True
server = ReusableTCPServer(("0.0.0.0", PORT), incoming)

print "Server listening on port %d" % PORT
server.serve_forever()
        
##WELCOME = """Welcome to paste palooza!
##Currently, only the file access feature is available.
##Access a file by entering its name: """
##
##name = raw_input(WELCOME)
##result = subprocess.check_output("cd pastes; ./nameaccess "+name+"; exit 0",\
##                                 stderr=subprocess.STDOUT,\
##                                 shell=True)
##
##print(result)
