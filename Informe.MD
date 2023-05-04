 #RECORDATORIOS/IDEAS: grafico x: tiempoDeSimulacion y1: paquetesEnviados y2: paquetesRecibidos

## Resumen
    En este trabajo se analizarán distintas simulaciones de un modelo de cola. Se buscará ver como diferentes variables afectan el funcionamiento de la red, además de implementar un algoritmo de control de flujo y congestión. 
    (TODO: RESULTADOS)


## Introduccion 
    Se denonima congestión al fenomeno que ocurre cuando, por distintas razones, a una red (o nodo) le está llegando mas datos de los que puede manejar.
    Por otro lado nos referimos a control de flujo al proceso de administrar la tasa de transmisión de datos entre un emisor y un receptor.

    Para el analisis se creó una red sencilla compuesta de un generador, una cola y un destino. Se definen ademas cuantos paquetes por segundo se pueden enviar y el tiempo de demora. 
    Creamos un nodo llamado nodeTx que contiene un generador y un buffer. El generador se encarga de crear y enviar paquetes; esto lo hace en intervalos aleatorios segun una distribución exponencial de parametro 'lambda', los almacena en su buffer en caso de no poder enviarlos.
    La cola intermedia recibe mensajes desde el nodeTx y los envia al nodeRx a ser consumidos, en caso de no poder cuenta con un buffer.
    Por su parte el nodo nodeRx cuenta con un destino y su buffer. Se encarga de consumir los paquetes recibidos 
    (TODO: HABLAR DE PAQUETES DESCARTADOS POR ESTAR LLENO EL BUFFER)


