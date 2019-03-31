#!/usr/bin/env python3
from flask import Flask, request, render_template
from connect import *

app = Flask(__name__)

@app.route('/', methods=['GET', 'POST'])
def entry():
    if request.method == "POST" :
        msg = request.form.get("message")
    return render_template('entry.html')

@app.route('/mixer', methods=['GET', 'POST'])
def mixer():
    if request.method == "POST" :
        d1 = request.form.get("Raw Drink No.1")
        d2 = request.form.get("Raw Drink No.2")
        d3 = request.form.get("Raw Drink No.3")
        d4 = request.form.get("Raw Drink No.4")
        msg = "1," + d1 + ".2," + d2 + ".3," + d3+ ".4," + d4 + ".done\n"
        with open("/root/ECE-477/Server/log/log.txt", "w") as f:
            f.write(msg)
    return render_template('mixer.html')


if __name__ == '__main__':
    # For pycharm, changing port and address must be done in app configuration.
    # Additional options: --host=0.0.0.0 --port=9090
    app.run(host="0.0.0.0", port="9090")
