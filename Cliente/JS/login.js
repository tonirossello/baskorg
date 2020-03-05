
var current = null;
var g_user;
var g_span;
var g_club;
var g_login_register = 0;

// Get the modal
var modal;


let socket = new WebSocket("ws://localhost:9990");

  socket.onopen = function(e) {

    loadjscssfile("login", "css");

  };

  //Lo que ejecutamos cuando se recibe un mensaje
  socket.onmessage = function(event) { 

    console.log("Mensaje recibido" + event.data);   
    
    var object = JSON.parse(event.data);   
    var JSONlegible = JSON.stringify(object,null,2); //pasamos a string el JSON
    //paragraph.textContent += JSONlegible; //lo mostramos en el cliente

    processMessage(object);
    
  };


  /* Función que ejecutamos pulsar el botón de logueo
  --> Envía un mensaje de tipo login al servidor */
  function botonLogin(){    
    var text = '{ "type":"login","user":"' +document.getElementById("user").value + '" , "pass":"'+document.getElementById("password").value + '"}';
    var object = JSON.parse(text);
    console.log(JSON.stringify(object,null,2));
    socket.send(text);
  }

  ///cambio de login a registro
  function show_register(){
    if (g_login_register == 0) {

      //vaciamos los campos de usuario y contraseña
      document.getElementById("rpassword").value = "";
      document.getElementById("ruser").value = "";

      //activamos el cuadro de registro y ocultamos el de iniciar sesión
      document.getElementById("iniciar_sessio").setAttribute("style","display:none;");
      document.getElementById("registre").setAttribute("style","display:block;");

      document.getElementById("text_box").textContent = "Registre";
      document.getElementById("text_action").textContent = "Ja tenc un compte";

      document.getElementById("tuser").setAttribute("style", "display:block;");
      document.getElementById("tpass").setAttribute("style", "display:block;");
      document.getElementById("ruser").setAttribute("style", "display:block;");      
      document.getElementById("rpassword").setAttribute("style", "display:block;");     
      document.getElementById("rsubmit").setAttribute("style", "display:block;");

      g_login_register = 1;

    } else {

      //vaciamos los campos de usuario y contraseña
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


  /* Función que ejecutamos pulsar el botón de registro
  --> Envía un mensaje de tipo createUser al servidor */
  function botonRegister(){

    var nom_usuari = document.getElementById("ruser").value;
    var password = document.getElementById("rpassword").value;

    //si no hay campos vacíos
    if ((nom_usuari != "") && (password != "")){

      var text ='{"type":"userCreate", "usuari":"' +nom_usuari + '", "password":"'+ password +'"}';
      socket.send(text);

    } else {

      error = document.getElementById("rerror");
      error.innerHTML = "Emplena tots els camps";

    } //end if

  }

  /* Función que ejecutamos cuando se recibe un mensaje del servidor
    --> Comprueba el campo type del JSON para llamar a su correspondiente función */
  function processMessage(object){

    switch(object.type){
        case "login": login(object); break;
        case "playersList": playersList(object); break;
        case "clubsList": load_clubs(object); break;
        case "clubCreate": modal_window(object); break;
        case "logout": logout(object); break;
        case "userCreate": userCreate(object); break;
        case "checkLoginStatus": checkLoginStatus(object); break;

    } //end switch
  }

  /* Función que ejecutamos cuando se recibe un mensaje respuesta de tipo login servidor
    --> Carga la pantalla home si el login ha sido válido */
  function login(object) {
    
    //si el login es válido
    if (String(object.operationSuccess).localeCompare("true") == 0){
      g_user = object.id_user;

      //cargamos los css correspondientes
      loadjscssfile("home", "css");
      loadjscssfile("menu", "css");

      document.getElementById("div_login").style.display = "none";
      document.getElementById("top").innerHTML = '<div><img src="./images/logoclub.jpg" alt="logo" class="center"/></div>'+
      '<div><ul><li><a onclick="refresh_clubs()">Inici</a></li><li><a>Administrar clubs</a></li><li><a>Contacte</a></li><li><a onclick="boton_logout()">Logout</a></li></ul></div>';
              
      //cargamos los clubes en la pantalla de inicio
      refresh_clubs();
      document.getElementById("login").disabled = true;
      enable_all_css();

    } else {
      error = document.getElementById("error");
      error.innerHTML = object.errorMessage;
      console.log("Mostrar error");
    }
  }

  /* Función que ejecutamos cuando se recibe un mensaje respuesta de tipo logout servidor
  --> Vacía todo el contenido HTML y habilita el div de login */
  function logout(object) {
    
    if (String(object.operationSuccess).localeCompare("true") == 0){

      document.getElementById("content").innerHTML = '';
      document.getElementById("top").innerHTML = '';
      document.getElementById("div_login").style.display = "";
      document.getElementById("login").disabled = false;
      disable_all_css();
      document.getElementById("user").value= "";
      document.getElementById("password").value= "";

    } //end if
  }

  /* Función que ejecutamos cuando se recibe un mensaje respuesta de tipo userCreate servidor
  --> Vacía todo el contenido del formulario de registro y muestra el mensaje de confirmación */
  function userCreate(object) {
    
    if (String(object.operationSuccess).localeCompare("true") == 0){

      document.getElementById("tuser").setAttribute("style", "display:none;");
      document.getElementById("tpass").setAttribute("style", "display:none;");
      document.getElementById("ruser").setAttribute("style", "display:none;");
      document.getElementById("rpassword").setAttribute("style", "display:none;");
      document.getElementById("rsubmit").setAttribute("style", "display:none;");
      document.getElementById("registre").setAttribute("style", "display:block;");   
      document.getElementById("left").innerHTML += '<div><a id="register_completed" style="margin-left:40px;">Enregistrament completat!</a></div>'  

    } //end if

  }

  function checkLoginStatus(object){
    console.log("Check");
    if (String(object.operationSuccess).localeCompare("true") == 0){ //ya logueado
      login(object);
    }
  }

  /* --> Carga de forma dinámica un archivo
    filename: nombre del archivo
    filetype: extensión del archivo        */
  function loadjscssfile(filename, filetype){

    //si el archivo no está cargado
    if (!already_charged(filename)){

      if (filetype=="js"){ 

        var fileref=document.createElement('script')
        fileref.setAttribute("type","text/javascript")
        fileref.setAttribute("src", "./JS/"+filename+"."+filetype)
        fileref.setAttribute("id", filename);

      }

      else if (filetype=="css"){

        var fileref=document.createElement("link")
        fileref.setAttribute("rel", "stylesheet")
        fileref.setAttribute("type", "text/css")
        fileref.setAttribute("href", "./CSS/"+filename+"."+filetype)
        fileref.setAttribute("id", filename);

      } //end if

      // añadimos el elemento link a la cabacera
      if (typeof fileref!="undefined")
          document.getElementsByTagName("head")[0].appendChild(fileref)
    } //end if
}

  /* --> Comprueba si un archivo css ya está cargado    */
function already_charged(filename){

  var links = document.getElementsByTagName("link");
  var i;

  for (i = 0; i < links.length; i++){

    var title = links[i].getAttribute("id");

    if ( title == filename){

        return true;

    } //end if
  } //end for

  return false;

}

/* --> Deshabilita todos los css excepto el de login    */
function disable_all_css(){

  var links = document.getElementsByTagName("link");
  var i;

  for (i = 0; i < links.length; i++){   

    var title = links[i].getAttribute("id");

    if ( title != "login"){

        links[i].setAttribute("disabled", "");

    } //end if
  } //end for

}

/* --> Habilita todos los css excepto el de login    */
function enable_all_css(){

  var links = document.getElementsByTagName("link");
  var i;

  for (i = 0; i < links.length; i++){

    var title = links[i].getAttribute("id");

    if ( title != "login"){

        links[i].removeAttribute("disabled");

    } //end if
  } //end for
}

/* Función que ejecutamos al pulsar el botón de para acceder a un club
--> Envía un mensaje de tipo playersList al servidor              */
function boton_accedir(id){
  var text = '{ "type":"playersList","user":"' +document.getElementById("user").value + '" , "id_club":"'+id + '"}';
  g_club = id;
  var object = JSON.parse(text);
    console.log(JSON.stringify(object,null,2));
    socket.send(text);
}

/* Función que ejecutamos al pulsar el logout de la barra del menú
--> Envía un mensaje de tipo logout al servidor              */
function boton_logout(){
  var text ='{"type":"logout"}';
  socket.send(text);
}

/* Función que ejecutamos cuando se recibe un mensaje respuesta de tipo playersList del servidor
--> Limpia el div content y genera el título, tabla y botón de crear usuario                  */
function playersList(object){

  document.getElementById("content").innerHTML = '';
  document.getElementById("content").innerHTML += '<h1 style="width:100%;">' + object.club_name+ '</h1>';
  
  document.getElementById("content").innerHTML += '<div style="width:50%; float:left; margin-bottom:30px;"><button  id="additional_buttons" style="background-color: #'+object.club_color+';'+
  '+margin-bottom: 25px;"> Gestor de comandes </button></div>';

  document.getElementById("content").innerHTML += '<div style="width:50%; float:left; margin-bottom:30px;"><button  id="additional_buttons" style="background-color: #'+object.club_color+';'+
  '+margin-bottom: 25px;"> Gestor d\'activitats </button></div>';
  document.getElementById("content").innerHTML += '<h2 style="margin-left:195px;"> Llistat de jugadors </h2>';
  
  generateTable(object);
  document.getElementById("content").innerHTML += '<div><button onclick = "boton_crear_jugador()" id="create_player" style="background-color: #'+object.club_color+';'+
  '+margin-bottom: 25px;"> + </button></div>';


}


/* --> Función que genera la tabla de jugadores del club abierto */
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

  th1.textContent = ('Nom'); tr.appendChild(th1);
  th2.textContent = ('DNI'); tr.appendChild(th2);
  th3.textContent = ('Soci'); tr.appendChild(th3);

  tbl.appendChild(tr);

  //cuerpo de la tabla
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

/*            Función que ejecutamos cuando se pulsa el botón crear club
--> Limpia el div content y genera el título, y el formulario de creación de club   */
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

/* Función que ejecutamos cuando se recibe un mensaje respuesta de tipo clubsList del servidor
--> Limpia el div content y crea todos los clubs que contiene el payload del mensaje          */
function load_clubs(object){

  document.getElementById("content").innerHTML = '';

  if (String(object.has_club).localeCompare("true") == 0){

    document.getElementById("content").innerHTML = '';
    document.getElementById("content").innerHTML += '<h1 style="width:100%;"> Els meus clubs </h1>';
    document.getElementById("content").innerHTML += '<div id="all_club_container">';

    var i;

    //recorremos todos los elementos del payload
        for (i = 0; i < object.payload.length; i++) {

          console.log("numero de clubs: "+ object.payload.length); 

          document.getElementById("all_club_container").innerHTML += '<div id="club_container"   style ="border: 2px solid #'+ object.payload[i].color_club+';">'+
          '<div id="inside_club"><h2 class="club_name">' + object.payload[i].nom_club +'</h2> <h3 class="club_code">#'+object.payload[i].codi_club+'</h3>'+
          '<button class="button" style ="background-color: #'+ object.payload[i].color_club+';" onclick="boton_accedir('+ object.payload[i].id_club +')">Accedir</button>'+
          '</div></div>';

        } //end for

        document.getElementById("content").innerHTML += '</div>';

    } //end if

    document.getElementById("content").innerHTML += '<div style="text-align:center;"><button class="button" id="crear_club" onclick="boton_crear_club()"> Crear club </button></div>';
      
}

/* Función que ejecutamos al enviar el formulario de creación del club
--> Envía un mensaje de tipo clubCreate al servidor                  */
function crear_club(){
  
  console.log("Crear club: "+ document.getElementById("club_name").value);
  var text = '{ "type":"clubCreate","club_name":"' +document.getElementById("club_name").value + '", "user":"'+g_user+'", "club_color":"' + document.getElementById("club_color").value.substring(1) + '"}';
  console.log(text);
  var object = JSON.parse(text); //???
  socket.send(text);

}

/* Función que ejecutamos al enviar el formulario de creación de jugador
--> Envía un mensaje de tipo playerCreate al servidor                   */
function crear_jugador(){

  if ((document.getElementById("fname").value != "") && (document.getElementById("fdni").value != "")){

    var text = '{ "type":"playerCreate","player_name":"' +document.getElementById("fname").value + '", "player_dni":"' +document.getElementById("fdni").value + 
    '", "player_soci":"' +document.getElementById("fsoci").value + '", "player_club":"'+g_club+'"}';
    socket.send(text);
    boton_accedir(g_club);

  } else {

    error = document.getElementById("etiqueta_error");
    error.innerHTML = "Emplena tots els camps";

  } //end if
}

/* Función que ejecutamos al pulsar el boton de '+' para añadir un jugador
--> Crea el formulario necesario para añadir nuevos jugadores             */
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

/* Función que ejecutamos al iniciar sesión o pulsar el botón de inicio del menú
--> Envía un mensaje de tipo clubsList al servidor              */
function refresh_clubs(){

  var text = '{ "type":"clubsList","id_user":"' + g_user + '"}';
  var pasar = JSON.parse(text);
  socket.send(text);  

}

/* Función que ejecutamos al crear un club correctamente
--> Envía un mensaje de tipo clubsList al servidor              */
function modal_window(object){
  
  document.getElementById("content").innerHTML +=  '<div id="myModal" class="modal">'+


  '<div class="modal-content">'+
    '<span class="close">&times;</span>'+
    '<p>'+ object.message +'</p>'+
  '</div>'+

'</div>';

  g_span = document.getElementsByClassName("close")[0];
  modal = document.getElementById("myModal");
  modal.style.display = "block";

  
  // When the user clicks on <span> (x), close the modal
  span.onclick = function() {
    modal.style.display = "none";
  }
}


// Función que cierra la ventana modal cuando se pulsa fuera de ella
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



