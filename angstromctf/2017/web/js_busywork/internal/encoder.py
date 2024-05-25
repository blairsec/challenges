import os
import random
import sys
import imp
import re

try:
    from jsmin import jsmin
except ImportError:
    def jsmin(js):
        return js

expr = re.compile(r'/\*{get[0-9]*}\*/')

def obfuscate(input_js, template_js, name, func):
    # Find all instances of the /*{get#}*/ text in the template
    instances = expr.findall(template_js)

    newjs = template_js[:]

    # Generate the text which will be interleaved
    out = []
    for instance in set(instances):
        # Find the number
        id = instance[6:-3]
        if id == '':
            id = 0
        id = int(id)
        newjs = newjs.replace(instance, func(id, input_js))

    return newjs


template_names = [filen[:-3] for filen in os.listdir("templates") if filen.endswith(".py")]

templates = dict()

for template in template_names:
    ns = dict()
    code = open("templates/{:s}.py".format(template)).read()
    exec(code, ns)
    templates[template] = ns['get']

titems = list(templates.items())

rounds = 2

def obst(infile, outfile):
    data = open(infile).read().replace("\n", "")

    for roundn, template in enumerate(titems *  int(rounds / len(titems))):
        jstemplate = open("templates/{:s}.js".format(template[0])).read().replace("\n", "")
        data = obfuscate(data, jstemplate, *template)
        data = jsmin(data)
        print("Obfuscated round {:d} of {:d} for '{:s}'".format(roundn + 1, rounds, infile))

    out = open(outfile, "w")
    out.write("var check = function() {")
    out.write(data.replace("\n", ""))
    out.write("}")
    out.close()

if __name__ == '__main__':
    from sys import argv
    if len(argv) == 1:
        obst("base.js", "obst.js")
    elif len(argv) == 3:
        obst(argv[1], argv[2])
    else:
        print("Usage: python encoder.py infile outfile")
