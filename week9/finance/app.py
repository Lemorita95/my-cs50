import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

import datetime
import pytz

# Configure application
app = Flask(__name__)

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/")
@login_required
def index():

    # get current portfolio of user -> amount of shares
    # does not display 0 shares symbols
    portfolio = db.execute(
        "SELECT symbol, SUM(shares) AS shares FROM transactions WHERE user_id = ? GROUP BY symbol HAVING SUM(shares) != 0", session["user_id"])

    # crete a variable that will get the total from shares
    total_shares = 0

    # get current quotation for each share
    for p in portfolio:
        # get symbol price
        p["price"] = lookup(p["symbol"])["price"]
        # get symbol total
        p["share_total"] = p["shares"] * p["price"]
        # add subtotal
        total_shares += p["share_total"]
        # format values
        p["price"] = usd(p["price"])
        p["share_total"] = usd(p["share_total"])

    cash = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])[0]["cash"]

    # format values
    total = usd(total_shares + cash)
    cash = usd(cash)

    return render_template("index.html", portfolio=portfolio, cash=cash, total=total)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # get users input
        symbol = request.form.get("symbol")
        shares = request.form.get("shares")

        # call for information
        quotation = lookup(symbol)

        # check for valid symbol
        if quotation == None:
            return apology("Invalid symbol")
        else:
            # return symbol price
            quotation = quotation["price"]

        # check for valid data format -> integers
        try:
            shares = int(shares)
        except ValueError:
            return apology("Invalid shares number")

        # check for valid values of shares -> greather then 0
        if int(shares) < 1:
            return apology("Invalid shares number")

        # check for users (session["user_id"]) balance
        balance = db.execute("SELECT * FROM users WHERE id = ?", session["user_id"])[0]["cash"]

        # check if user can afford amount of shares @ price
        if quotation * shares > balance:
            return apology("Cannot afford")

        # get current timestamp for transaction
        timestamp = datetime.datetime.now(pytz.timezone("US/Eastern")).strftime("%Y-%m-%d %H:%M:%S")

        # calculate user balance after share buying
        final_balance = balance - quotation * shares

        # record transaction
        # buys are included as positive amount of shares
        # only update user balance if transaction is done
        _ = db.execute("INSERT INTO transactions (symbol, shares, price, datetime, user_id) VALUES(?, ?, ?, ?, ?)",
                   symbol.upper(), shares, quotation, timestamp, session["user_id"])
        # update user balance
        if _:
            db.execute("UPDATE users SET cash = ? WHERE id = ?", final_balance, session["user_id"])

        # return to home
        # flash message
        flash("Purchased!")
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():

    # get users transactions
    transactions = db.execute(
        "SELECT symbol, shares, price, datetime FROM transactions WHERE user_id = ?", session["user_id"])

    return render_template("history.html", transactions=transactions)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute(
            "SELECT * FROM users WHERE username = ?", request.form.get("username")
        )

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(
            rows[0]["hash"], request.form.get("password")
        ):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():

    # if user submit form @ quote.html
    if request.method == "POST":

        # get users input
        symbol = request.form.get("symbol")

        # call for information
        quotation = lookup(symbol)

        # check for valid symbol
        if quotation == None:
            return apology("Invalid symbol")
        else:
            # return symbol price
            quotation = quotation["price"]

        # show user quotation
        return render_template("quoted.html", symbol=symbol, quotation=quotation)

    # if user goes to quote.html page
    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    # if user submit form @ register.html
    if request.method == "POST":

        # gather active users - only when performing a form submission
        users = db.execute("SELECT username FROM users")

        # check if field is not null
        if not request.form.get("username"):
            return apology("Must provide username")

        # check for already taken usernames
        if users:
            if request.form.get("username") in users[0]["username"]:
                return apology("Username already taken")

        # check if password and confirmation are not null
        if not request.form.get("password") or not request.form.get("confirmation"):
            return apology("Must provide password and/or confirmation")

        # check if password and confirmation match
        if not (request.form.get("password") == request.form.get("confirmation")):
            return apology("Password and confirmation must match")

        # add user to database
        db.execute("INSERT INTO users (username, hash) VALUES(?, ?)", request.form.get(
            "username"), generate_password_hash(request.form.get("password")))

        # redirect to login page
        return redirect("/login")

    # if user made thorught register.html via click or redirect
    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():

    # get current portfolio from user
    portfolio = db.execute(
        "SELECT symbol, SUM(shares) AS shares FROM transactions WHERE user_id = ? GROUP BY symbol HAVING SUM(shares) != 0", session["user_id"])

    # turn into a dict to handle validations
    portfolio_dict = {s["symbol"]: s["shares"] for s in portfolio}

    # if user submit form @ sell.html
    if request.method == "POST":

        # get users input
        symbol = request.form.get("symbol")
        shares = request.form.get("shares")

        # check if field is not null
        if not symbol:
            return apology("Must select a Symbol")

        # check if input are in users portfolio
        if not (symbol in portfolio_dict.keys()):
            return apology("You dont own this share")

        # check for valid data format -> integers
        try:
            shares = int(shares)
        except ValueError:
            return apology("Invalid shares number")

        # check for valid values of shares -> greather then 0
        if int(shares) < 1:
            return apology("Invalid shares number")

        # check if user has that many shares
        if shares > portfolio_dict[symbol]:
            return apology("Too many shares")

        # get current price of symbol
        quotation = lookup(symbol)

        # check for valid symbol
        if quotation == None:
            return apology("Invalid symbol")
        else:
            # return symbol price
            quotation = quotation["price"]

        # check for users (session["user_id"]) balance
        balance = db.execute("SELECT * FROM users WHERE id = ?", session["user_id"])[0]["cash"]

        # get current timestamp for transaction
        timestamp = datetime.datetime.now(pytz.timezone("US/Eastern")).strftime("%Y-%m-%d %H:%M:%S")

        # calculate user balance after share buying
        final_balance = balance + quotation * shares

        # record transaction
        # sells are includes with negative amount of shares
        # only update user balance if transaction is done
        _ = db.execute("INSERT INTO transactions (symbol, shares, price, datetime, user_id) VALUES(?, ?, ?, ?, ?)",
                   symbol.upper(), -shares, quotation, timestamp, session["user_id"])
        # update user balance
        if _:
            db.execute("UPDATE users SET cash = ? WHERE id = ?", final_balance, session["user_id"])

        # return home
        # flash message
        flash("Sold!")
        return redirect("/")

    # if user made thorught sell.html via click or redirect
    else:
        return render_template("sell.html", portfolio=portfolio)


@app.route("/add-money", methods=["GET", "POST"])
@login_required
def add_money():

    # user current balance
    cash = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])[0]["cash"]

    # if user submit form @ add-money.html
    if request.method == "POST":

        money = request.form.get("money")

        # check for null values
        if not money:
            return apology("Insert some amount")

        try:
            money = float(money)
        except ValueError:
            return apology("Invalid number")

        if (money < 0):
            return apology("Invalid amount")

        # calculate user balance after share buying
        final_balance = cash + money

        # update user balance
        db.execute("UPDATE users SET cash = ? WHERE id = ?", final_balance, session["user_id"])

        return redirect("/")

    # if user made thorught add-money.html via click or redirect
    else:
        return render_template("add_money.html", cash=cash)
