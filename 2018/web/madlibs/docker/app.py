from flask import Flask, render_template, render_template_string, send_from_directory, request
from jinja2 import Environment, FileSystemLoader
from time import gmtime, strftime
template_dir = './templates'
env = Environment(loader=FileSystemLoader(template_dir))


madlib_names = ["The Tale of a Person","A Random Story"]
story_fields = {
    "The Tale of a Person":['Author Name','Adjective','Noun','Verb'],
    "A Random Story":['Author Name','Adjective','Noun','Any first name','Verb']
    }

app = Flask(__name__)
app.secret_key = open("flag.txt").read()

@app.route("/",methods=["GET"])
def home():
    return render_template("home.html",libs=madlib_names)

@app.route("/form/<templatename>",methods=["GET"])
def madlib(templatename):
    global madlib_names
    if templatename in madlib_names:
        return render_template("home.html",libs=madlib_names,title=templatename,fields=story_fields[templatename])
    else:
        error_message = 'The MadLib with title "' + templatename + '" could not be found.'
        return render_template("home.html",libs=madlib_names,message=error_message)

@app.route("/result/<templatename>",methods=["POST"])
def output(templatename):

    if templatename not in madlib_names:
        return "Template not found."

    inpValues = []
    for i in range(len(story_fields[templatename])):
        if not request.form[str(i+1)]:
            return "All form fields must be filled"
        else:
            inpValues.append(request.form[str(i+1)][:24])

    authorName = inpValues.pop(0)[:12]
    try:
        comment = render_template_string('''This MadLib with title %s was created by %s at %s''' % (templatename, authorName, strftime("%Y-%m-%d %H:%M:%S", gmtime())))
    except:
        comment = "Error generating comment."
    return render_template("_".join(templatename.lower().split())+".html",libtitle=templatename,footer=comment, libentries=inpValues)


@app.route("/get-source", methods=["GET","POST"])
def source():
    return send_from_directory('./','app.py')

if __name__ == "__main__":
    app.run(host='0.0.0.0', port=7777, threaded=True)
