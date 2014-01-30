/* canvas erstellen */
var canvas = document.getElementById("myCanvas");
var ctx=canvas.getContext("2d");

/* Andere globale Variablen */
var consoleLog = document.getElementById("consoleLog");
var webSocket;
var heizung_ist_defaultwert = 0;
var kronleuchter = document.getElementById("slider_kronleuchter");
var lampe = document.getElementById("slider_lampe");
var tv = document.getElementById("label_tv");
var heizung_soll = document.getElementById("slider_heizung_soll");
var alarm_active = document.getElementById("checkbox_alarm_active");
var alarm = document.getElementById("label_alarm");
var snd = new Audio("../multimedia/alarm.ogg"); // buffers automatically when created
//var address = "ws://echo.websocket.org";
//var address = "ws://" + "147.87.174.91" + ":5000";
var address = "ws://" + location.host + ":5000";

//var rangeslider = document.getElementByID("rangevalue");

/* Handler fuer Mouse-click im canvas-Bereich */ 
function on_canvas_click(ev) {
  // ... x,y sind die absoluten Koordinaten innerhalb des Canvas, scrollOffset berücksichtigen
  var x = ev.clientX - canvas.offsetLeft + document.body.scrollLeft + document.documentElement.scrollLeft;
  var y = ev.clientY - canvas.offsetTop + document.body.scrollTop + document.documentElement.scrollTop;
  
    
  /* wenn auf Kronleuchter gedrückt */
  if(x > 196 && x < (196 + 22) && y > 156 && y < (156 + 22)){
		if(kronleuchter.value < 1){
			kronleuchter.value = 100;
		}
		else{
			kronleuchter.value = 0;
		}
		kronleuchter.onchange();
        //kronleuchter_slider_callback(kronleuchter.value);
        logToConsole('<span style="color: green;">Kronleuchter: ' + kronleuchter.value+'</span>');
	}
	
	/* wenn auf Lampe gedrückt */
  if(x > 248 && x < (248 + 21) && y > 196 && y < (196 + 17)){
		if(lampe.value < 1){
			lampe.value = 100;
		}
		else{
			lampe.value = 0;
		}
		lampe.onchange();
        //lampe_slider_callback(lampe.value);
		
        logToConsole('<span style="color: green;">Lampe: ' + lampe.value+'</span>');
	}
	
		/* wenn auf TV gedrückt */
  if(x > 164 && x < (163 + 23) && y > 227 && y < (227 + 14)){
        if(tv.value === "OFF"){
            tv.value = "ON";
        }
        else{
            tv.value = "OFF";
        }
        tv_change_callback(tv.value);
		
		logToConsole('<span style="color: green;">TV: ' + tv.value+'</span>');
	}
}

function tv_change_value(value){
    /* Bild zeichnen */
    var tvbild = new Image();
    
    if(value === "OFF"){
        tvbild.src = "../bilder/TV_off.png";
    }
    else{
        tvbild.src = "../bilder/TV_on.png";
    }
        
    tvbild.onload = function() {
        ctx.drawImage(tvbild, 163, 227, 23, 14);
    };
}

function tv_change_callback(value){
    tv_change_value(value);
    /* Hier Wert an BeagleBone senden */
    send_tv_pair(value);
}

function lampe_change_value(slideAmount){
	/* Bild zeichnen */
	var img_lampe_on = new Image();
	var img_lampe = new Image();
	
	/* altes Bild loeschen */
	img_lampe.src = "../bilder/Lampe_off.png";
	img_lampe.onload = function() {
		ctx.drawImage(img_lampe, 248, 195, 21, 17);
	};
	img_lampe_on.src = "../bilder/Lampe_on.png";
	img_lampe_on.onload = function() {
		/* Transparenz setzen */
		ctx.globalAlpha = slideAmount/100.0;
		ctx.drawImage(img_lampe_on, 248, 195, 21, 17);
		ctx.globalAlpha = 1.0;
	};
}

/* Handler für Ständerlampe, GUI aktualisieren und Kommunikation zum Huesli */
 function lampe_slider_callback(slideAmount){
    lampe_change_value(slideAmount);
	/* Hier Wert an BeagleBone senden */
    send_lamp_pair(slideAmount);
 }
 
 function kronleuchter_change_value(slideAmount){
 	/* Bild zeichnen */
	var img_lampe_on = new Image();
	var img_lampe = new Image();
	
	/* altes Bild loeschen */
	img_lampe.src = "../bilder/Kronleuchter_off.png";
	img_lampe.onload = function() {
		ctx.drawImage(img_lampe, 196, 156, 22, 22);
	};
	img_lampe_on.src = "../bilder/Kronleuchter_on.png";
	img_lampe_on.onload = function() {
		/* Transparenz setzen */
		ctx.globalAlpha = slideAmount/100.0;
		ctx.drawImage(img_lampe_on, 196, 156, 22, 22);
		ctx.globalAlpha = 1.0;
	};
 }
 
 /* Handler für Kronleuchter, GUI aktualisieren und Kommunikation zum Huesli */
 function kronleuchter_slider_callback(slideAmount){
    kronleuchter_change_value(slideAmount);
	/* Hier Wert an BeagleBone senden */
	send_kronleuchter_pair(slideAmount);
 }
 
 /* Handler für Heizungsslider, Kommunikation zum Huesli */
 function heizung_soll_slider_callback(slideAmount){
	/* Hier Wert an BeagleBone senden */
	sendHeizungPair(slideAmount);
 }

heizung_ist_change(heizung_ist_defaultwert);

/* Illustrates the Heating control value */
 function heizung_ist_change(stellwert){
	/* Bild rechts zeichnen */
	var img_heating_on = new Image();
	var img_heating = new Image();
	
	/* altes Bild loeschen */
	img_heating.src = "../bilder/Heizung_off.png";
	img_heating.onload = function() {
		ctx.drawImage(img_heating, 278, 236, 9, 30);
	};
	img_heating_on.src = "../bilder/Heizung_on.png";
	img_heating_on.onload = function() {
		/* Transparenz setzen */
		ctx.globalAlpha = stellwert/100.0;
		ctx.drawImage(img_heating_on, 278, 236, 9, 30);
		ctx.globalAlpha = 1.0;
	};
	
	/* Bild links zeichnen */
	var img_heating_on_left = new Image();
	var img_heating_left = new Image();
	/* altes Bild loeschen */
	img_heating_left.src = "../bilder/Heizung_off_left.png";
	img_heating_left.onload = function() {
		ctx.drawImage(img_heating_left, 132, 237, 9, 30);
	};
	img_heating_on_left.src = "../bilder/Heizung_on_left.png";
	img_heating_on_left.onload = function() {
		/* Transparenz setzen */
		ctx.globalAlpha = stellwert/100.0;
		ctx.drawImage(img_heating_on_left, 132, 237, 9, 30);
		ctx.globalAlpha = 1.0;
	};
 }
 
/* Handler für Alarm-aktivierung, GUI aktualisieren und Kommunikation zum Huesli */
 function alarm_active_callback(value){
    /* no special functionality here */
 }
 

function play_alarm(){
    /* Play sound */
 	alarm.value = "ON";
	snd.load();
	snd.play();
	snd.addEventListener('ended',  function(){snd.play();});
}
 
 function reset_alarm(){
    /* stop sound */
	alarm.value = "OFF";
	
	snd.pause();
 }
 
 /* Handler fure Mouse-click registrieren */ 
canvas.addEventListener('click', on_canvas_click, false);

/* Abschliessend Bild zeichnen */
var cat = new Image();
cat.src = "../bilder/haus.gif";
cat.onload = function() {
  ctx.drawImage(cat, 0, 0, 400, 300);
};

/* Log Console */
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

/* test the modal dialog execution using javascript */
function openModalDialog()
{
    // neccessary to avoid "#openModal" showing up in the URL
    restoreUrl = location.href;
    // jump to anchor defined in html_canvas -> div id = "openModal"
    location.href = "#openModal";
    // neccessary to avoid "#openModal" showing up in the URL
    history.replaceState(null,null,restoreUrl);
}

/* Websocket Management */
initSocket();

/* Handle a F5 (refresh) or tab close on browser --> close Websocket! */
window.onbeforeunload = function() {
    /* disable onclose handler first */
    websocket.onclose = function () {};
    websocket.close()
};

function initSocket(){
    webSocket = new WebSocket(address);
    
    webSocket.onopen = function (evt){ logToConsole("CONNECTED to " + address + ": " + evt.data); };
    webSocket.onerror = function (evt){ logToConsole('<span style="color: red;">ERROR on ' + address + ':</span> ' + evt.data); };
    webSocket.onmessage = function (evt){ onMessage(evt); };
    webSocket.onclose = function (evt){ logToConsole("DISCONNECTED: " + evt.data); };
}

function onMessage(evt)
{
    logToConsole('<span style="color: blue;">RESPONSE: ' + evt.data+'</span>');
    
    /* Es könnten mehrere Pairs in einer JSON Message sein, also damit rechnen */
    var jsonObject = JSON.parse(evt.data);

    // Special Client INIT values:
    /*
    if(jsonObject.TV){
        tv.value = jsonObject.TV;
        tv_change_value(tv.value);
    }
    if(jsonObject.Lampe){
        lampe.value = jsonObject.Lampe;
        lampe_change_value(jsonObject.Lamp);
    }
    if(jsonObject.Leuchter){
        kronleuchter.value = jsonObject.Leuchter;
        kronleuchter_change_value(jsonObject.Leuchter);
    }
    if(jsonObject.TempSoll){
        heizung_soll.value = jsonObject.TempSoll;
        heizung_soll_change_value(jsonObject.TempSoll);
    }*/
    
    // Normal Server INFO values:
    if(jsonObject.TempIst){
        label_temp_ist.value = jsonObject.TempSoll;
    }
    if(jsonObject.Heizung){
        heizung_ist_change(jsonObject.TempSoll);
    }
    if(jsonObject.Burglar){
        if(jsonObject.Burglar === 1 && alarm_active.checked){
            /* if light barrier is broken AND the alarm is activated, alarm! */
            openModalDialog();
            play_alarm();
        }
    }
}

function send_tv_pair(val){
    var sendObj = { TV: val };
    webSocket.send(JSON.stringify(sendObj));
    logToConsole("SENT: " + JSON.stringify(sendObj));
}

function send_lamp_pair(val){
    var sendObj = { Lampe: val };
    webSocket.send(JSON.stringify(sendObj));
    logToConsole("SENT: " + JSON.stringify(sendObj));
}

function send_kronleuchter_pair(val){
    var sendObj = { Leuchter: val };
    webSocket.send(JSON.stringify(sendObj));
    logToConsole("SENT: " + JSON.stringify(sendObj));
}

function sendHeizungPair(val){
    var sendObj = { TempSoll: val };
    webSocket.send(JSON.stringify(sendObj));
    logToConsole("SENT: " + JSON.stringify(sendObj));
}

function sendAlarmResetPair(){
    var sendObj = { AlarmReset: "" };
    webSocket.send(JSON.stringify(sendObj));
    logToConsole("SENT: " + JSON.stringify(sendObj));
}

function sendAlarmPair(val){
    var sendObj = { Alarm: val };
    webSocket.send(JSON.stringify(sendObj));
    logToConsole("SENT: " + JSON.stringify(sendObj));
}
