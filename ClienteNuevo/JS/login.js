
var current = null;
var user;



document.querySelector('#user').addEventListener('focus', function(e) {
  if (current) current.pause();
  current = anime({
    targets: 'path',
    strokeDashoffset: {
      value: 0,
      duration: 700,
      easing: 'easeOutQuart'
    },
    strokeDasharray: {
      value: '240 1386',
      duration: 700,
      easing: 'easeOutQuart'
    }
  });
});
document.querySelector('#password').addEventListener('focus', function(e) {
  if (current) current.pause();
  current = anime({
    targets: 'path',
    strokeDashoffset: {
      value: -336,
      duration: 700,
      easing: 'easeOutQuart'
    },
    strokeDasharray: {
      value: '240 1386',
      duration: 700,
      easing: 'easeOutQuart'
    }
  });
});
document.querySelector('#submit').addEventListener('focus', function(e) {
  if (current) current.pause();
  current = anime({
    targets: 'path',
    strokeDashoffset: {
      value: -730,
      duration: 700,
      easing: 'easeOutQuart'
    },
    strokeDasharray: {
      value: '530 1386',
      duration: 700,
      easing: 'easeOutQuart'
    }
  });
});


let socket = new WebSocket("wss://localhost:9990");

  socket.onopen = function(e) {
    //lert("[open] Connection established");
    //alert("Sending to server");
    //socket.send("Conexión abierta");
    
  };

  socket.onmessage = function(event) { //Lo que ejecutamos cuando se recibe un mensaje
    console.log("Mensaje recibido" + event.data);   
    //paragraph = document.getElementById("serverResponse");
    //paragraph.innerHTML=''; //reseteamos el párrafo

    var object = JSON.parse(event.data);   
    var JSONlegible = JSON.stringify(object,null,2); //pasamos a string el JSON
    //paragraph.textContent += JSONlegible; //lo mostramos en el cliente

    processMessage(object);
    
  };

  /*socket.onclose = function(event) {
    if (event.wasClean) {
      alert(`[close] Connection closed cleanly, code=${event.code} reason=${event.reason}`);
    } else {
      alert('[close] Connection died');
    }
  };*/

  socket.onerror = function(error) {
    alert(`[error] ${error.message}`);
  };

  function botonLogin(){    
    var text = '{ "type":"login","user":"' +document.getElementById("user").value + '" , "pass":"'+document.getElementById("password").value + '"}';
    var object = JSON.parse(text);
    console.log(JSON.stringify(object,null,2));
    socket.send(text);
  }

  function processMessage(object){

    switch(object.type){
        case "login": login(object); break;
        case "playersList": playersList(object); break;
        case "clubsList": load_clubs(object); break;
        
        //default: console.log("Tipo no definido"); break;
    }
  }

  function login(object) {
    
    if (String(object.operationSuccess).localeCompare("true") == 0){
      user = object.id_user;

      loadjscssfile("home", "css");
      loadjscssfile("menu", "css");

      document.getElementById("div_login").style.display = "none";
      document.getElementById("top").innerHTML = '<div><img src="./images/logoclub.jpg" alt="logo" class="center"/></div>'+
      '<div><ul><li><a class="active" onclick="refresh_clubs()">Inici</a></li><li><a href="#contact">Contacte</a></li><li><a href="#about">Sobre nosaltres</a></li></ul></div>';
              
      //cargamos los clubes 
      load_clubs(object);
      document.getElementById("login").disabled = true;

    } else {

      error = document.getElementById("error");
      error.innerHTML = object.errorMessage;
      console.log("Mostrar error");
    }
  }

  function loadjscssfile(filename, filetype){
    if (filetype=="js"){ //if filename is a external JavaScript file
        var fileref=document.createElement('script')
        fileref.setAttribute("type","text/javascript")
        fileref.setAttribute("src", filename)
        fileref.setAttribute("id", filename);
    }
    else if (filetype=="css"){ //if filename is an external CSS file
        var fileref=document.createElement("link")
        fileref.setAttribute("rel", "stylesheet")
        fileref.setAttribute("type", "text/css")
        fileref.setAttribute("href", "./CSS/"+filename+"."+filetype)
        fileref.setAttribute("id", filename);

        console.log(fileref.getAttribute("href"));
    }
    if (typeof fileref!="undefined")
        document.getElementsByTagName("head")[0].appendChild(fileref)
}

function boton_accedir(id){
  var text = '{ "type":"playersList","user":"' +document.getElementById("user").value + '" , "id_club":"'+id + '"}';
  var object = JSON.parse(text);
    console.log(JSON.stringify(object,null,2));
    socket.send(text);
}


function playersList(object){
  console.log("Players list");
  document.getElementById("content").innerHTML = '';
  document.getElementById("content").innerHTML += '<h1 style="width:100%;"> Nom del club </h1>';
  generateTable(object);
}

function generateTable(object){
  loadjscssfile("table", "css");
  var content = document.getElementById('content');
  var tbl = document.createElement('table');
  tbl.id = "table";
  tbl.style.width = '100%';

  var rows = object.total;

  var td, tr, th1,th2,th3;

  ///Cabecera de la tabla
  tr = document.createElement('tr');
  
  th1 = document.createElement('th');
  th2 = document.createElement('th');
  th3 = document.createElement('th');

  th1.textContent = ('Nom');
  tr.appendChild(th1);
  th2.textContent = ('DNI');
  tr.appendChild(th2);
  th3.textContent = ('Soci');
  tr.appendChild(th3);
  tbl.appendChild(tr);

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
  content.appendChild(tbl)
}

function boton_crear_club(){
    document.getElementById("content").innerHTML = '';

    document.getElementById("content").innerHTML += '<div class="container">'+

  
      '<label for="club_name">Nom del club</label>'+
      '<input type="text" id="club_name" name="club_name" placeholder="Escriu el nom del club.."/>'+
      '<label for="club_name">Color</label>'+
      '<input type="color" id="club_color" name="club_color" minlength="6" maxlength="6" placeholder="Color Hex.."/>'+
      '<button onclick="crear_club()"> Crear </button>'+

  '</div>';

}

function load_clubs(object){
  document.getElementById("content").innerHTML = '';
  console.log("HAS club: " + object.has_club);
  if (String(object.has_club).localeCompare("true") == 0){
    document.getElementById("content").innerHTML = '';
    document.getElementById("content").innerHTML += '<h1 style="width:100%;"> Els meus clubs </h1>';
    var i;
    document.getElementById("content").innerHTML += '<div id="all_club_container">';

        for (i = 0; i < object.payload.length; i++) {
          console.log("numero de clubs: "+ object.payload.length); 

          document.getElementById("all_club_container").innerHTML += '<div id="club_container"   style ="border: 2px solid #'+ object.payload[i].color_club+';">'+
          '<div id="inside_club"><h2 class="club_name">' + object.payload[i].nom_club +'</h2> <h3 class="club_code">#'+object.payload[i].codi_club+'</h3></div>'+
          '<button class="button" style ="background-color: #'+ object.payload[i].color_club+';" onclick="boton_accedir('+ object.payload[i].id_club +')">Accedir</button>'+
          '</div>';
        }

        document.getElementById("content").innerHTML += '</div>';
      } 

      document.getElementById("content").innerHTML += '<div style="text-align:center;"><button class="button" id="crear_club" onclick="boton_crear_club()"> Crear club </button></div>';
      
}

function crear_club(){
  console.log("Crear club: "+ document.getElementById("club_name").value);
  var text = '{ "type":"clubCreate","club_name":"' +document.getElementById("club_name").value + '", "user":"'+user+'", "club_color":"' + document.getElementById("club_color").value.substring(1) + '"}';
  console.log(text);
  var object = JSON.parse(text); //???
  socket.send(text);
}

function refresh_clubs(){
  var text = '{ "type":"clubsList","id_user":"' + user + '"}';
  var pasar = JSON.parse(text);
  socket.send(text);  
}
