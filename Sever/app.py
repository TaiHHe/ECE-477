from flask import Flask, request, render_template
from connect import *

app = Flask(__name__)

@app.route('/', methods=['GET', 'POST', 'PUT'])
def entry():
    if request.method == "POST" :
        msg = request.form.get("message")
        mySocket = listen_port()
        client = connect_to_esp8266(mySocket)
        send_to_esp8266(msg, client)
    return render_template('entry.html')


if __name__ == '__main__':
    # For pycharm, changing port and address must be done in app configuration.
    app.run()
