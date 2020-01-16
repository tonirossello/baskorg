var logged = false;
var tipoRecibido;
var serverResponse;

function updateLoginLabel(){
    var currentLoginState = document.getElementById("Estado");
    if (!logged){
        currentLoginState.innerHTML="No logueado";
    } else {
        currentLoginState.innerHTML="Estás logueado";
    }
}


  let socket = new WebSocket("ws://127.0.0.1:9990");

  socket.onopen = function(e) {
    //alert("[open] Connection established");
    //alert("Sending to server");
    //socket.send("Conexión abierta");
  };

  socket.onmessage = function(event) { //Lo que ejecutamos cuando se recibe un mensaje
    console.log("Mensaje recibido" + event.data);   
    paragraph = document.getElementById("serverResponse");
    paragraph.innerHTML=''; //reseteamos el párrafo

    var object = JSON.parse(event.data);   
    var JSONlegible = JSON.stringify(object,null,2); //pasamos a string el JSON
    paragraph.textContent += JSONlegible; //lo mostramos en el cliente

    processMessage(object);
    
  };

  socket.onclose = function(event) {
    if (event.wasClean) {
      alert(`[close] Connection closed cleanly, code=${event.code} reason=${event.reason}`);
    } else {
      alert('[close] Connection died');
    }
  };

  socket.onerror = function(error) {
    alert(`[error] ${error.message}`);
  };

  

  function processMessage(object){
    tipoRecibido = document.getElementById("TipoRecibido");
    switch(object.type){
        case "login": console.log("tipo login");  changeLoginStatus(object); break;
        case "logout": console.log("tipo logout");  changeLoginStatus(object); break;
        //default: console.log("Tipo no definido"); break;
    }

    tipoRecibido.innerHTML= object.type;
  }

  function changeLoginStatus(object){
      if (object.operationSuccess){
         if (logged){
           logged = false;         
         } else {
           logged = true;
           generateTable();
         }
      }
      updateLoginLabel();
  }

  function generateTable(){
    console.log("generate tables");
    var table = document.getElementById("playersList");
    var rows = 3;
    var cols = 3;
    
    for (var r = 0; r< rows; r++){
      table += '<tr>';
        for(var c=1; c<= cols; c++)
        {
          table += '<td>' + c + '</td>';
        }
      table += '</tr>';
    }

    document.write('<table border=1>' + table + '</table>');
  }

  