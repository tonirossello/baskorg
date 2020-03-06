 
***********************************************
# Readme

## Descripción del proyecto

El proyecto consiste en una aplicación multiplataforma de gestión de clubes deportivos. El target son los miembros de las juntas directivas de los clubes, así como sus posibles coordinadores deportivos. 

Actualmente se trata de una versión muy inicial de la idea que tengo como aplicación “final”. Su funcionalidad no está completa, ya que está en proceso de desarrollo. 

El proyecto tiene una parte cliente que ha sido desarrollada en HTML, CSS y Javascript. Esta parte es la que ve el usuario final de la aplicación. También dispone de una parte servidor, que es el encargado de recibir y procesar las peticiones que le realizan desde el cliente. Esta parte ha sido desarrollada en QT (C++).

## Requisitos

### Git

Desde el inicio del proyecto he ido utilizando un pendrive para poder implementar un sistema de control de versiones a la vez que trabajaba en dos sitios diferentes. Cada vez que consideraba que había algún cambio importante realizaba un ‘commit -a’ indicando las modificaciones más destacadas que se habían hecho en esta versión. 

En la última semana se ha corrompido un archivo del git y no me ha permitido hacer el último commit, que sería el siguiente: 
Implementación de tests de algunas funciones de la aplicación


### Documentación

Para documentar el proyecto hemos utilizado Doxygen. Es una herramienta muy sencilla que nos permite documentar el código siguiendo algunas estructuras de comentarios. Por ejemplo: @param para los parámetros de una función.
Además, se puede añadir información en la página principal de la documentación, así como la versión del documento entre otras muchas cosas. 


### Envío de mensajes JSON

Normalmente casi todas las acciones que realice el usuario requieren de una respuesta del servidor. Por ello, al realizar una acción como por ejemplo, clicar en el botón de login, el cliente envía un mensaje JSON al servidor indicando el tipo de petición (type = “login”), y las credenciales introducidas por el usuario. El servidor contestará a la petición con otro mensaje JSON.

Para el uso del JSON en el servidor hemos necesitado la siguiente librería: 
https://github.com/nlohmann/json

### Conexión a una base de datos

El programa guarda los datos necesarios para su funcionamiento en una base de datos Mysql. Elegí este tipo de base de datos porque ya tenía una versión hecha y la pude aprovechar.

El cliente nunca interactúa de forma directa con la base de datos, de eso se encarga el servidor. Es decir, si un usuario se quiere dar de alta, se enviará el JSON correspondiente al servidor, que se encargará de hacer la petición necesaria a la base de datos para llevar a cabo el alta. 

La base de datos tiene dos triggers: 

  * Creación de club: genera un código alfanumérico aleatorio de 8 caracteres que será usado en un futuro para poder unirse al club y poder ver/hacer acciones en función de los permisos otorgados por el administrador. 
  * Creación de usuario: cuando se crea un usuario se genera un UUID aleatorio y se encripta su contraseña.

Para poder utilizar el programa debe crearse el usuario 'pitu' en mysql con la contraseña 'pitu'. La base de datos deberá tener el nombre cbsacabaneta. Todos los datos anteriores pueden ser modificados en el archivo config.prop

### Traducciones

Se pedía que el programa implementara soporte para traducciones. Se han realizado traducciones en los mensajes de error del login. Se detecta el idioma del sistema operativo para enviar el mensaje en el idioma que toca. En el futuro se pueden añadir muchas más traducciones.

### Tests

Se ha realizado un programa a parte para testear la aplicación, es decir, para comprobar que futuros cambios en el código fuente del servidor no afecten al funcionamiento correcto del programa. 

Se ha importado la cabecera de la aplicación del servidor para testear las funciones reales que se utilizan. Además, también se crea una base de datos alternativa para hacer las pruebas. 

Para poder hacer los tests con la base de datos de prueba se deberá crear una base de datos con el mismo usuario y contraseña definido en el apartado de la base de datos. El nombre de la base 
de datos de prueba debe ser: "testDB"


### Websockets

Usamos la tecnología websocket para dispones de un canal bidireccional de comunicación entre cliente y servidor. Para ello usamos una librería externa descargada de github. (https://github.com/machinezone/IXWebSocket) 

### Certificado digital

También se pedía que hubiera una conexión segura entre cliente y servidor. Para ello se ha utilizado TLS, que es una versión más segura y actualizada del SSL.
Usuarios de prueba


### Usuarios de prueba

Al importar la base de datos que se encuentra en la carpeta BBDD, se disponen de dos usuarios de prueba:

Usuario: usuario, contraseña: usuario
Usuario: sinclub, contraseña: sinclub

El segundo no tiene ningún club creado.


### Guía de estilo

  * Se ha utilizado el prefijo "m_" para las variables de módulo. 
  * Se ha utilizado el prefijo "g_" para las variables globales.
  * Los nombres de las funciones que se ejecutan al recibir un JSON son iguales al campo "type" del JSON.

### Github

    Este repositorio está publicado en github: https://github.com/tonirossello/baskorg
    
### Opinión personal

Creo que el hecho de poder llevar a cabo nuestro propio proyecto con una serie de requisitos mínimos me ha ayudado a que esté motivado en hacer la práctica. Al final si te imponen hacer algo que realmente no te hace ilusión habría costado mucho más dedicarle horas a este trabajo.

Una de las cosas que más me ha gustado ha sido desarrollar el programa en dos partes (cliente y servidor). Es de lo más habitual que cualquier aplicación tenga estas dos partes, pero hasta la fecha no lo había hecho y me apetecía probarlo. 

A pesar de estar satisfecho con la versión muy inicial del programa que he hecho, me habría gustado haber tenido más tiempo para sacar una versión todavía mejor. Cuando terminemos los exámenes tendré más tiempo para poder seguir con el desarrollo hasta llegar a una versión más completa.

Una cosa que me hubiera gustado hacer, aunque puede conllevar gastos, era publicar la aplicación el algún sitio y poder acceder a ella desde cualquier lugar. 

En conclusión, este proyecto me ha ayudado a entender muchos conceptos que me servirán en el futuro en cualquier proyecto que haga, además de ser la primera versión de mi futura aplicación.

***********************************************