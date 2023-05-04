 #RECORDATORIOS/IDEAS: grafico x: tiempoDeSimulacion y1: paquetesEnviados y2: paquetesRecibidos
# INFORME

## Resumen
En este trabajo se analizarán distintas simulaciones de un modelo de cola. Se buscará ver como diferentes variables afectan el funcionamiento de la red, además de implementar un algoritmo de control de flujo y congestión. 
(TODO: RESULTADOS)


## Introduccion 
Se denonima congestión al fenomeno que ocurre cuando, por distintas razones, a una red (o nodo) le está llegando mas datos de los que puede manejar. Por otro lado nos referimos a control de flujo al proceso de administrar la tasa de transmisión de datos entre un emisor y un receptor.

Para el analisis se creó una red sencilla compuesta de un generador, una cola y un destino. Se definen ademas cuantos paquetes por segundo se pueden enviar y el tiempo de demora. 

Creamos un nodo llamado nodeTx que contiene un generador y un buffer. El generador se encarga de crear y enviar paquetes; esto lo hace en intervalos aleatorios segun una distribución exponencial de parametro 'lambda', los almacena en su buffer en caso de no poder enviarlos.

La cola intermedia recibe mensajes desde el nodeTx y los envia al nodeRx a ser consumidos, en caso de no poder cuenta con un buffer.

Por su parte el nodo nodeRx cuenta con un destino y su buffer. Se encarga de consumir los paquetes a medida que los va recibiendo y usa el buffer para ir encolando los siguientes.  

Planteamos dos casos:
    - Caso 1: La velocidad de transmisión desde el nodeTx al queue es de 1 Mbps, luego desde la queue al nodeRx es de 1 Mbps y consume el paquete a 0,5 Mbps.
    - Caso 2:La velocidad de transmisión desde el nodeTx al queue es de 1 Mbps, luego desde la queue al nodeRx es de 0,5 Mbps y consume el paquete a 1 Mbps.

Y para cada caso probamos diferentes 'lambdas' para variar el tiempo de generación de paquetes.

Para poder visualizar las situaciones que pasarian usamos simulaciones discretas con la herramienta Omnet++, de esas visualizaciones sacamos los siguientes datos en base al tiempo de simulación:
    - Paquetes enviados
    - Paquetes recibidos
    - Paquetes descartados
    - Delay de cada paquete
    - Cantidad de paquetes en buffers

En ambos casos vimos que se pierden una importante cantidad de paquetes, debido a saturaciónes en distintos buffers según el caso. En el caso 1 se satura el buffer de nodeRx (receptor) mientras que en el caso 2 se satura el buffer de Queue (la red). 

Aca vemos que como se llenan los buffers según el caso:
<img src="casosLab3/caso1/0.1/caso1_Buffers.png" caption="Caso 1"/>
<img src="casosLab3/caso2/0.1/caso2_Buffers.png" caption="Caso 2"/>

En las siguientes imagenes vemos que cuando se llena el buffer empiezan a perderse paquetes:
<img src="casosLab3/caso1/0.1/caso1_0.1_perdidosrelacionbuffer.png" caption="Caso 1"/>
<img src="casosLab3/caso2/0.1/caso2_0.1_BufferPaquetesPerdidos" caption="Caso 2"/>

Vemos que como hay un cuello de botella por reducir a la mitad la velocidad en una parte, distinta según el caso, provoca que a larga se pierdan la mitad de los paquetes enviados. Esto se ve en los siguientes gráficos:
<img src="casosLab3/caso1/0.1/caso1_0.1_Barrapaquetesenviadosyrecibidos.png" caption="Caso 1"/>
<img src="casosLab3/caso2/0.1/caso2_0.1_Barrapaquetesrecibidosyenviados.png" caption="Caso 2"/>