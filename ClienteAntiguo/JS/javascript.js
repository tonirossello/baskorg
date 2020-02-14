var logged = false;
var tipoRecibido;
var serverResponse;

function updateLoginLabel(){
    console.log("Actualizamos label");
    var currentLoginState = document.getElementById("Estado");
    if (!logged){
        currentLoginState.innerHTML="No logueado";
    } else {
        currentLoginState.innerHTML="Estás logueado";
    }
}


  let socket = new WebSocket("wss://localhost:9990");

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
        case "playersList": console.log("tipo playerList"); generateTable(object); break;
        //default: console.log("Tipo no definido"); break;
    }

    tipoRecibido.innerHTML= object.type;
  }

  function changeLoginStatus(object){

    var success = String(object.operationSuccess).localeCompare("true") == 0;

      if (success){
         if (logged){
           logged = false;         
         } else {
           logged = true;
         }
         
         updateLoginLabel();
      }
      
  }

  /*function generateTable(object){
    //console.log("generate tables");
    var table = document.getElementById("playersList");
    var cols = 3;
    var rows = object.total;
    
    for (var r = 0; r< rows; r++){
      table += '<tr>';
        for(var c=0; c< cols; c++)
        {
          if (c==0) table += '<td>' + object.payload[r].nom + '</td>';
          if (c==1) table += '<td>' + object.payload[r].dni + '</td>';
          if (c==2) table += '<td>' + object.payload[r].soci + '</td>';
        }
      table += '</tr>';
    }

    document.write('<table border=1>' + table + '</table>');
  }*/
  function generateTable(object){
    var body = document.getElementsByTagName('body')[0];
    var tbl = document.createElement('table');
    tbl.style.width = '100%';
    tbl.setAttribute('border', '2');
    var rows = object.total;

    var td, tr;
    var tbdy = document.createElement('tbody');
    for (var r = 0; r < rows; r++) {
      tr = document.createElement('tr');
      for (var c = 0; c < 3; c++) {    
          td = document.createElement('td');
          if (c==0) td.textContent = object.payload[r].nom;
          if (c==1) td.textContent = object.payload[r].dni;
          if (c==2) td.textContent = object.payload[r].soci;
          tr.appendChild(td)     
      }
      tbdy.appendChild(tr);
    }
    tbl.appendChild(tbdy);
    body.appendChild(tbl)
}

  