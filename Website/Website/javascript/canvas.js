/* canvas erstellen */
var canvas = document.getElementById("myCanvas");
var ctx=canvas.getContext("2d");

/* Andere globale Variablen */
var consoleLog = document.getElementById("consoleLog");

//var rangeslider = document.getElementByID("rangevalue");

/* Handler fuer Mouse-click im canvas-Bereich */ 
function on_canvas_click(ev) {
  // ... x,y sind die absoluten Koordinaten innerhalb des Canvas
  var x = ev.clientX - canvas.offsetLeft;
  var y = ev.clientY - canvas.offsetTop;
  var kl = document.getElementById("s_kronleuchter");
  var lp = document.getElementById("s_lampe");
  var tv_var = document.getElementById("tv");
  
  var message = "Mouse-Position: x=" + x + "y=" + y;
  logToConsole("Mouse-Position: x=" + x + "y=" + y);
  
  ctx.font="14px Arial";
  ctx.clearRect(0,300,400,50);  /* alten Text loeschen */
  ctx.fillStyle = "#000000";
  ctx.fillText(message,10,315);  /* neuen Text schreiben */
  message = "Kronleuchter: " +kronleuchter.value + " Lampe : " + lampe.value + " Heizung Soll: " +heizung.value +" Ist: " + heizung_ist.value;
  ctx.fillText(message, 10, 330);
  
  
  /* wenn auf Kronleuchter gedrückt */
  if(x > 196 && x < (196 + 22) && y > 156 && y < (156 + 22)){
		
		if(kl.value < 1){
			kl.value = 100;
		}
		else{
			kl.value = 0;
		}
		kl.onchange();
        
        logToConsole('<span style="color: blue;">Kronleuchter: ' + kronleuchter.value+'</span>');
	}
	
	/* wenn auf Lampe gedrückt */
  if(x > 248 && x < (248 + 21) && y > 196 && y < (196 + 17)){
		
		if(lp.value < 1){
			lp.value = 100;
		}
		else{
			lp.value = 0;
		}
		lp.onchange();
        
        logToConsole('<span style="color: blue;">Lampe: ' + lampe.value+'</span>');
	}
	
		/* wenn auf TV gedrückt */
  if(x > 164 && x < (163 + 23) && y > 227 && y < (227 + 14)){
		var tvbild = new Image();
		if(tv.value == "OFF"){
			tv.value = "ON";
			tvbild.src = "../bilder/TV_on.png";
		}
		else{
			tv.value = "OFF";
			tvbild.src = "../bilder/TV_off.png";
		}
		
		tvbild.onload = function() {
			ctx.drawImage(tvbild, 163, 227, 23, 14);
		};	
		
		logToConsole('<span style="color: blue;">TV: ' + tv.value+'</span>');
        
        sendTvPair(tv.value);
	}
	
} 
/* Handler für Ständerlampe, GUI aktualisieren und Kommunikation zum Huesli */
 function lamp_change(slideAmount){
	
	/* Bild zeichnen */
	var img_lampe_on = new Image();
	var img_lampe = new Image();
	
	/* altes Bild loeschen */
	img_lampe.src = "../bilder/Lampe_off.png";
	img_lampe.onload = function() {
		ctx.drawImage(img_lampe, 248, 196, 21, 17);
	};
	img_lampe_on.src = "../bilder/Lampe_on.png";
	img_lampe_on.onload = function() {
		/* Transparenz setzen */
		ctx.globalAlpha = slideAmount/100.0;
		ctx.drawImage(img_lampe_on, 248, 196, 21, 17);
		ctx.globalAlpha = 1.0;
	};
	/* Hier Wert an BeagleBone senden */
    sendLampPair(slideAmount);
 }
 
 /* Handler für Kronleuchter, GUI aktualisieren und Kommunikation zum Huesli */
 function kron_change(slideAmount){
	
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
	/* Hier Wert an BeagleBone senden */
	sendLeuchterPair(slideAmount);
 }
 
 function heizung_soll(slideAmount){
	/* Hier Wert an BeagleBone senden */
	sendHeizungPair(slideAmount);
 }
 

 
/* Handler fure Mouse-click registrieren */ 
canvas.addEventListener('click', on_canvas_click, false);



/* Bild zeichnen */
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

/* Websocket Management */
function sendTvPair(val){

}

function sendLampPair(val){

}

function sendLeuchterPair(val){

}

function sendHeizungPair(val){

}
