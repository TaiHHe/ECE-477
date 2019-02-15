from flask import Flask, request, render_template
from connect import *

app = Flask(__name__)

@app.route('/', methods=['GET', 'POST', 'PUT'])
def entry():
    if request.method == "POST" :
        mySocket = listen_port()
        client = connect_to_esp8266(mySocket)
        msg = request.form.get("message")
        send_to_esp8266(msg, client)
    return render_template('entry.html')


if __name__ == '__main__':
    # For pycharm, changing port and address must be done in app configuration.
    # Additional options: --host=0.0.0.0 --port=9090
    app.run()
