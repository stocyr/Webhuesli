var webSocket;
var consoleLog;
var sendBox;
var wsEchoURI = "ws://echo.websocket.org/";

function onPageLoad() {
    consoleLog = document.getElementById("consoleLog");
    sendBox = document.getElementById('data');
}

function ConnectPressed(){
    //alert("trying to connect...");
    var host = document.getElementById('host_name').value;
    var port = document.getElementById('port_number').value;
    
    //webSocket = new WebSocket('127.0.0.1:5000', 'webhuesli-protocol');
    //webSocket = new WebSocket('ws://' + location.hostname, 'webhuesli-protocol');
    //webSocket = new WebSocket('ws://' + host + ':' + port, 'webhuesli-protocol');
    webSocket = new WebSocket(wsEchoURI);
    
    //var jsonObject1 = {"cmd":"set","dlampe":"58"};
    //var jsonObject2 = JSON.parse("String");
    //var string = JSON.stringify(jsonObject1);
    //var dlampvalue = jsonObject1.dlampe;
    
    webSocket.onopen = function (evt){ onOpen(evt) };
    webSocket.onerror = function (evt){ onError(evt) };
    webSocket.onmessage = function (evt){ onMessage(evt) };
    webSocket.onclose = function (evt){ onClose(evt) };
}

function DisconnectPressed(){
    webSocket.close();
}

function onOpen(evt)
{
    //alert("socket has been opened!" + " Location=" + location.host);
    logToConsole("CONNECTED");
}

function onError(evt)
{
    //alert("socket error! " + evt);
    logToConsole('<span style="color: red;">ERROR:</span> ' + evt.data);
}

function onMessage(evt)
{
    //alert("message received: " + evt.data + "!");
    logToConsole('<span style="color: blue;">RESPONSE: ' + evt.data+'</span>');
}

function onClose(evt)
{
    //alert("socket has been closed!");
    logToConsole("DISCONNECTED");
}

function SendPressed(){
    //alert(sendBox.value + " was sent.");
    webSocket.send(sendBox.value);
    logToConsole("SENT: " + sendBox.value);
}

function logToConsole(message)
{
    var pre = document.createElement("p");
    pre.style.wordWrap = "break-word";
    pre.innerHTML = message;
    consoleLog.appendChild(pre);
    
    while (consoleLog.childNodes.length > 50)
    {
      consoleLog.removeChild(consoleLog.firstChild);
    }
    
    consoleLog.scrollTop = consoleLog.scrollHeight;
}

function ClearLogPressed()
{
    while (consoleLog.childNodes.length > 0)
    {
        consoleLog.removeChild(consoleLog.lastChild);
    }
}

window.addEventListener("load", onPageLoad, false);