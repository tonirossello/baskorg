var current = null;
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
        //default: console.log("Tipo no definido"); break;
    }
  }

  function login(object) {
    if (String(object.operationSuccess).localeCompare("true") == 0){
      console.log("Cambiar a home");
      loadjscssfile("home", "css")
      //document.write('<link rel="stylesheet" type="text/css" href="../CSS/home.css">');
      document.getElementById("login").style.display = "none";
      document.getElementById("content").innerHTML = '<div><img src="./images/logoclub.jpg" alt="logo" class="center"/></div>'+
      '<div><ul><li><a class="active" href="./HTML/home.html">Inici</a></li><li><a href="#contact">Contacte</a></li><li><a href="#about">Sobre nosaltres</a></li></ul></div>'+

  '<h1 style="width:100%;"> Els meus clubs </h1>'+

  '<div id="all_club_container">'+
     '<div id="club_container">'+
          '<div id="inside_club"><h2 class="club_name">Bàsquet Sa Cabaneta</h2> <h3 class="club_code">#cab12345</h3></div>'+
          '<button class="button">Accedir</button>'+
      '</div>'+

      '<div id="club_container">'+
          '<div id="inside_club"><h2 class="club_name">Bàsquet Sa Cabaneta</h2> <h3 class="club_code">#cab12345</h3></div>'+
          '<button class="button">Accedir</button>'+
      '</div>'+
  '</div>';
      
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
    }
    else if (filetype=="css"){ //if filename is an external CSS file
        var fileref=document.createElement("link")
        fileref.setAttribute("rel", "stylesheet")
        fileref.setAttribute("type", "text/css")
        fileref.setAttribute("href", "./CSS/"+filename+"."+filetype)

        console.log(fileref.getAttribute("href"));
    }
    if (typeof fileref!="undefined")
        document.getElementsByTagName("head")[0].appendChild(fileref)
}
