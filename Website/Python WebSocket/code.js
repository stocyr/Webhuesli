var webSocket;

function ConnectPressed(){
    alert("trying to connect...");
    var host = document.getElementById('host_name').value;
    var port = document.getElementById('port_number').value;
    
    //webSocket = new WebSocket('127.0.0.1:5000', 'webhuesli-protocol');
    //webSocket = new WebSocket('ws://' + location.hostname, 'webhuesli-protocol');
    //webSocket = new WebSocket('ws://' + host + ':' + port, 'webhuesli-protocol');
    webSocket = new WebSocket('ws://html5rocks.websocket.org/echo', 'soap', 'xmpp');
    
    //var jsonObject1 = {"cmd":"set","dlampe":"58"};
    //var jsonObject2 = JSON.parse("String");
    //var string = JSON.stringify(jsonObject1);
    //var dlampvalue = jsonObject1.dlampe;
    
    webSocket.onopen = function (){
        alert("socket has been opened!");
    }
    webSocket.onerror = function (error){
        alert("socket error! " + error);
    }
    webSocket.onmessage = function (msg){
        alert("message received: \"" + msg.data " "\"!");
    }
    webSocket.onclose = function (){
        alert("socket has been closed!");
    }
}

function DisconnectPressed(){
    webSocket.close();
}

function SendPressed(){
    webSocket.send(document.getELementById('data').value;
}