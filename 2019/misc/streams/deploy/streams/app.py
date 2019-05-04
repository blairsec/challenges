from flask import *

app = Flask(__name__)
    
@app.route("/", methods=['GET'])
def home():
    return render_template("index.html")

@app.route("/video/<name>", methods=['GET','POST'])
def video(name):
    return send_from_directory("video", name)

@app.errorhandler(500)
def internalSeverError(e):
    return render_template("500.html")

@app.errorhandler(404)
def notFound(e):
    return render_template("404.html")

if __name__ == "__main__":
    app.run()

