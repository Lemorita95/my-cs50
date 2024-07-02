from flask import Flask, redirect, render_template, request, session
from flask_session import Session

app = Flask(__name__)

app.config["SESSION_PERMANENT"] = False #session cookie, when closed tends to be deleted
app.config["SESSION_TYPE"] = "filesystem" #ensures 'shopping cart' stores in server files not cookie istelf
Session(app)

@app.route("/")
def index():
    return render_template("index.html", name=session.get("name"))

@app.route("/login", methods=["GET", "POST"])
def login():
    if request.method == "POST":
        session["name"] = request.form.get("name")
        return redirect("/")
    return render_template("login.html")

@app.route("/logout")
def logout():
    session.clear()
    return redirect("/")
