from flask import Flask, request, render_template
from connect import *

app = Flask(__name__)

@app.route('/', methods=['GET', 'POST'])
def entry():
    if request.method == "POST" :
        mySocket = listen_port()
        client = connect_to_esp8266(mySocket)
        msg = request.form.get("message")
        send_to_esp8266(msg, client)
    return render_template('entry.html')

@app.route('/mixer', methods=['GET', 'POST'])
def mixer():
    if request.method == "POST" :
        mySocket = listen_port()
        client = connect_to_esp8266(mySocket)
        d1 = request.form.get("Raw Drink No.1")
        d2 = request.form.get("Raw Drink No.2")
        d3 = request.form.get("Raw Drink No.3")
        d4 = request.form.get("Raw Drink No.4")
        msg = "1," + d1 + ".2," + d2 + ".3," + d3+ ".4," + d4 + ".done"
        send_to_esp8266(msg, client)
    return render_template('mixer.html')


if __name__ == '__main__':
    # For pycharm, changing port and address must be done in app configuration.
    # Additional options: --host=0.0.0.0 --port=9090
    app.run(host="0.0.0.0", port="9090")
