# Tp1-Taller

**Nombre:**  Santiago Pablo Fernandez Caruso

**Padron:** 105267

**Repositorio:** https://github.com/SaFernandezC/Tp1-Taller



## Descripcion

El presente trabajo consta de la implementacion de un programa cliente / servidor el cual debe sigue el protocolo descripto en el enunciado y su funcion es encriptar lineas de texto.  

Para la implementacion se utilizaron diferentes TDA de forma de encapsular y modularizar de forma correcta. Por el lado del servidor se utiliza el  tda socket, tda server y las funciones de encriptacion se encuentran en server_cipher. En cuanto al cliente, se utiliza el tda protocol y el mismo tda socket utilizado en el servidor.

El tda socket es el encargado de realizar la conexion con el servidor (cuando se lo usa del lado del usuario), y tambien de enviar y recibir los mensajes (bytes).





## Diagramas

A continuacion, un diagrama de 1 iteracion (tanto lo que se ejecuta del lado del cliente como lo del servidor va en un loop) de la secuencia de envio y recibo de mensajes. Es un diagrama simplificado (no se incluyen los parametros ni los retornos en las funciones) para no entorpecer la lectura del mismo.

![](img/Secuencia_while.png)



## Aclaraciones

Para la realizacion del trabajo se uso como "guia" el ejemplo de cliente-servidor brindado por la catedra.