
var current = null;
var g_user;
var span;
var g_club;
var g_login_register = 0;

// Get the modal
var modal;

// Get the button that opens the modal
/*var btn = document.getElementById("myBtn");*/



let socket = new WebSocket("wss://localhost:9990");

  socket.onopen = function(e) {
    //lert("[open] Connection established");
    //alert("Sending to server");
    //socket.send("Conexión abierta");
    loadjscssfile("login", "css");
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

  function show_register(){
    if (g_login_register == 0) {

      document.getElementById("iniciar_sessio").setAttribute("style","display:none;");
      document.getElementById("registre").setAttribute("style","display:block;");

      document.getElementById("text_box").textContent = "Registre";
      document.getElementById("text_action").textContent = "Ja tenc un compte";

      document.getElementById("tuser").setAttribute("style", "display:block;");
      document.getElementById("tpass").setAttribute("style", "display:block;");
      document.getElementById("ruser").setAttribute("style", "display:block;");      
      document.getElementById("rpassword").setAttribute("style", "display:block;");     
      document.getElementById("rsubmit").setAttribute("style", "display:block;");

      document.getElementById("rpassword").value = "";
      document.getElementById("ruser").value = "";

      g_login_register = 1;

    } else {

      document.getElementById("user").value = "";
      document.getElementById("password").value = "";

      document.getElementById("iniciar_sessio").setAttribute("style","display:block;");
      document.getElementById("registre").setAttribute("style","display:none;");
      document.getElementById("text_box").textContent = "Iniciar sessió";
      document.getElementById("text_action").textContent = "No tenc compte, em vull enregistrar";

      var registered_message = document.getElementById("register_completed");
      if (registered_message)  document.getElementById("register_completed").innerHTML = '';
     
      g_login_register = 0;

    }
  }

  function botonRegister(){
    var nom_usuari = document.getElementById("ruser").value;
    var password = document.getElementById("rpassword").value;
    console.log(nom_usuari +" " +password);
    if ((nom_usuari != "") && (password != "")){
      var text ='{"type":"userCreate", "usuari":"' +nom_usuari + '", "password":"'+ password +'"}';
      socket.send(text);
    } else {
      error = document.getElementById("rerror");
      error.innerHTML = "Emplena tots els camps";
    }
  }

  function processMessage(object){

    switch(object.type){
        case "login": login(object); break;
        case "playersList": playersList(object); break;
        case "clubsList": load_clubs(object); break;
        case "clubCreate": modal_window(object); break;
        case "logout": logout(object); break;
        case "userCreate": userCreate(object); break;
        
        //default: console.log("Tipo no definido"); break;
    }
  }

  function login(object) {
    
    if (String(object.operationSuccess).localeCompare("true") == 0){
      g_user = object.id_user;

      loadjscssfile("home", "css");
      loadjscssfile("menu", "css");

      document.getElementById("div_login").style.display = "none";
      document.getElementById("top").innerHTML = '<div><img src="./images/logoclub.jpg" alt="logo" class="center"/></div>'+
      '<div><ul><li><a onclick="refresh_clubs()">Inici</a></li><li><a onclick="boton_logout()">Logout</a></li></ul></div>';
              
      //cargamos los clubes 
      load_clubs(object);
      document.getElementById("login").disabled = true;
      enable_all_css();

    } else {

      error = document.getElementById("error");
      error.innerHTML = object.errorMessage;
      console.log("Mostrar error");
    }
  }

  function logout(object) {
    
    if (String(object.operationSuccess).localeCompare("true") == 0){
      document.getElementById("content").innerHTML = '';
      document.getElementById("top").innerHTML = '';
      document.getElementById("div_login").style.display = "";
      document.getElementById("login").disabled = false;
      disable_all_css();
      document.getElementById("user").value= "";
      document.getElementById("password").value= "";
    }
  }

  function userCreate(object) {
    
    if (String(object.operationSuccess).localeCompare("true") == 0){
      document.getElementById("tuser").setAttribute("style", "display:none;");
      document.getElementById("tpass").setAttribute("style", "display:none;");
      document.getElementById("ruser").setAttribute("style", "display:none;");
      document.getElementById("rpassword").setAttribute("style", "display:none;");
      document.getElementById("rsubmit").setAttribute("style", "display:none;");
      document.getElementById("registre").setAttribute("style", "display:block;");   
      document.getElementById("left").innerHTML += '<div><a id="register_completed" style="margin-left:40px;">Enregistrament completat!</a></div>'  
    }
  }

  function loadjscssfile(filename, filetype){
    if (!already_charged(filename)){
      if (filetype=="js"){ //if filename is a external JavaScript file
          var fileref=document.createElement('script')
          fileref.setAttribute("type","text/javascript")
          fileref.setAttribute("src", "./JS/"+filename+"."+filetype)
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
}

function already_charged(filename){
  var links = document.getElementsByTagName("link");
  var i;
  for (i = 0; i < links.length; i++){
    var title = links[i].getAttribute("id");
    if ( title == filename){
        return true;
    }
  } 
  return false;
}

function disable_all_css(){
  var links = document.getElementsByTagName("link");
  var i;
  for (i = 0; i < links.length; i++){    
    var title = links[i].getAttribute("id");
    if ( title != "login"){
        links[i].setAttribute("disabled", "");
    }
  } 
}

function enable_all_css(){
  var links = document.getElementsByTagName("link");
  var i;
  for (i = 0; i < links.length; i++){
    var title = links[i].getAttribute("id");
    if ( title != "login"){
        links[i].removeAttribute("disabled");
    }
  } 
}

function boton_accedir(id){
  var text = '{ "type":"playersList","user":"' +document.getElementById("user").value + '" , "id_club":"'+id + '"}';
  g_club = id;
  var object = JSON.parse(text);
    console.log(JSON.stringify(object,null,2));
    socket.send(text);
}

function boton_logout(){
  var text ='{"type":"logout"}';
  socket.send(text);
}


function playersList(object){
  console.log("Players list");
  document.getElementById("content").innerHTML = '';
  document.getElementById("content").innerHTML += '<h1 style="width:100%;">' + object.club_name+ '</h1>';
  generateTable(object);
  document.getElementById("content").innerHTML += '<div><button onclick = "boton_crear_jugador()" id="create_player" style="background-color: #'+object.club_color+'; margin-bottom: 25px;"> + </button></div>';
}

function generateTable(object){
  loadjscssfile("table", "css");
  var content = document.getElementById('content');
  var tbl = document.createElement('table');
  tbl.id = "table";
  tbl.style.width = '80%';
  

  var rows = object.total;

  var td, tr, th1,th2,th3;

  ///Cabecera de la tabla
  tr = document.createElement('tr');
  tr.setAttribute('style', 'background-color: #'+object.club_color+';');
  
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
    loadjscssfile("form", "css");
    document.getElementById("content").innerHTML = '';
    document.getElementById("content").innerHTML += '<h1 style="width:100%;"> Crea un nou club </h1>';

    document.getElementById("content").innerHTML += '<div class="club_creation">'+

  
      '<label for="club_name">Nom del club</label>'+
      '<input type="text" id="club_name" name="club_name" placeholder="Escriu el nom del club.."/> <br>'+
      '<label for="club_name">Color</label>'+
      '<input type="color" id="club_color" name="club_color" minlength="6" maxlength="6" placeholder="Color Hex.."/> <br>'+
      '<button id="boton_crear" onclick="crear_club()"> Crear </button>'+

  '</div>';

}

function load_clubs(object){
  document.getElementById("content").innerHTML = '';
  if (String(object.has_club).localeCompare("true") == 0){
    document.getElementById("content").innerHTML = '';
    document.getElementById("content").innerHTML += '<h1 style="width:100%;"> Els meus clubs </h1>';
    var i;
    document.getElementById("content").innerHTML += '<div id="all_club_container">';

        for (i = 0; i < object.payload.length; i++) {
          console.log("numero de clubs: "+ object.payload.length); 

          document.getElementById("all_club_container").innerHTML += '<div id="club_container"   style ="border: 2px solid #'+ object.payload[i].color_club+';">'+
          '<div id="inside_club"><h2 class="club_name">' + object.payload[i].nom_club +'</h2> <h3 class="club_code">#'+object.payload[i].codi_club+'</h3>'+
          '<button class="button" style ="background-color: #'+ object.payload[i].color_club+';" onclick="boton_accedir('+ object.payload[i].id_club +')">Accedir</button>'+
          '</div></div>';

        }

        document.getElementById("content").innerHTML += '</div>';
      } 

      document.getElementById("content").innerHTML += '<div style="text-align:center;"><button class="button" id="crear_club" onclick="boton_crear_club()"> Crear club </button></div>';
      
}

function crear_club(){
  
  console.log("Crear club: "+ document.getElementById("club_name").value);
  var text = '{ "type":"clubCreate","club_name":"' +document.getElementById("club_name").value + '", "user":"'+g_user+'", "club_color":"' + document.getElementById("club_color").value.substring(1) + '"}';
  console.log(text);
  var object = JSON.parse(text); //???
  socket.send(text);

}

function crear_jugador(){
  if ((document.getElementById("fname").value != "") && (document.getElementById("fdni").value != "")){
    var text = '{ "type":"playerCreate","player_name":"' +document.getElementById("fname").value + '", "player_dni":"' +document.getElementById("fdni").value + '", "player_soci":"' +document.getElementById("fsoci").value + '", "player_club":"'+g_club+'"}';
    console.log("Legal" +text);
    socket.send(text);
    boton_accedir(g_club);
  } else {
    console.log("MuestraError");
    error = document.getElementById("etiqueta_error");
    error.innerHTML = "Emplena tots els camps";
  }
}

function boton_crear_jugador(){

  loadjscssfile("form", "css");
  document.getElementById("content").innerHTML += 
  
    '<div class = "player_form">'+

        '<label for="fname">Nom i llinatges</label>'+
        '<input type="text" id="fname" name="name">'+

        '<label for="dni">DNI</label>'+
        '<input type="text" id="fdni" name="dni" pattern="[0-9]{8}[A-Za-z]{1}">'+

        '<label for="soci">Soci</label>'+
        '<select id="fsoci" name="soci">'+
          '<option value="Individual">Individual</option>'+
          '<option value="Familar">Familar</option>'+
          '<option value="No soci" selected="selected">No</option>'+
        '</select>'+
      
        '<input type="submit" value="Crear jugador" onclick="crear_jugador()">'+
        '<div style="text-align:center;"><label id="etiqueta_error"></label></div>'
    '</div>';

}

function refresh_clubs(){
  var text = '{ "type":"clubsList","id_user":"' + g_user + '"}';
  var pasar = JSON.parse(text);
  socket.send(text);  
}

function modal_window(){
  
  document.getElementById("content").innerHTML +=  '<div id="myModal" class="modal">'+


  '<div class="modal-content">'+
    '<span class="close">&times;</span>'+
    '<p>Club creat correctament</p>'+
  '</div>'+

'</div>';
  span = document.getElementsByClassName("close")[0];
  modal = document.getElementById("myModal");
  modal.style.display = "block";

  
  // When the user clicks on <span> (x), close the modal
  span.onclick = function() {
    modal.style.display = "none";
  }
}


// When the user clicks anywhere outside of the modal, close it
window.onclick = function(event) {
  if (event.target == modal) {
    modal.style.display = "none";
  }
}


///Animación de las ventanas de registro y login
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

document.querySelector('#ruser').addEventListener('focus', function(e) {
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

document.querySelector('#rpassword').addEventListener('focus', function(e) {
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

document.querySelector('#rsubmit').addEventListener('focus', function(e) {
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



