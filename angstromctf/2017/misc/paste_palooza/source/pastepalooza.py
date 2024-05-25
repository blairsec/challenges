import subprocess

WELCOME = """Welcome to paste palooza!
Currently, only the file access feature is available.
Access a file by entering its name: """

name = raw_input(WELCOME)
result = subprocess.check_output("cd pastes; ./nameaccess "+name+"; exit 0",\
                                 stderr=subprocess.STDOUT,\
                                 shell=True)

print(result)
