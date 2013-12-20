var webSocket;
var consoleLog;
var sendBox;
var wsEchoURI = "ws://echo.websocket.org";


var consoleLog = document.getElementById("consoleLog");
var sendBox = document.getElementById('data');
var host = document.getElementById('host_name').value;

ConnectPressed();   // already connect on load page

function ConnectPressed(){
    //webSocket = new WebSocket('127.0.0.1:5000', 'webhuesli-protocol');
    //webSocket = new WebSocket('ws://' + location.hostname, 'webhuesli-protocol');
    //webSocket = new WebSocket('ws://' + host + ':' + port, 'webhuesli-protocol');
    webSocket = new WebSocket(host);
    
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