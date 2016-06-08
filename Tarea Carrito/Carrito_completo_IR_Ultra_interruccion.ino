#include <IRremote.h>//libreria para usar sensores IR qe permite recibir o transmitir códigos de control remoto por infrarrojos.

IRrecv irrecv(2);//variable al qe se le pasa el numero del pin donde esta el IRresector,Crear el objeto receptor, utilizando un nombre de su elección.
decode_results results;

long distancia; //varia qe guarda la distancia
long tiempo;
int OUT = 5; //Pin TRIGGER
int IN = 6; //Pin ECHO
int revoluciones= 150;//valor de las revoluciones qe recibiran los motores


void setup()
{
  pinMode(13, OUTPUT);//Salida del pin activada - pin del motor izquierdo
  pinMode(12, OUTPUT);//Salida del pin activada - pin del motor izquierdo
  pinMode(9, OUTPUT);//Salida del pin activada - pin del motor derecho
  pinMode(8, OUTPUT);//Salida del pin activada - pin del motor derecho
  pinMode(4,OUTPUT);//Salida del pin activada - Pin del Led qe detecta las señales del IR emisor.
  pinMode(IN, INPUT);//Salida del pin activada - pin del ECHO del sensor ultrasonido
  pinMode(OUT, OUTPUT);//Salida del pin activada - pin del TRIGGER del sensor ultrasonido
  Serial.begin(9600);//Establece la velocidad de datos en bits por segundo (baudios) para la transmisión de datos en serie.
  irrecv.enableIRIn();//Iniciar el proceso de recepción.
  attachInterrupt(0, control, FALLING);//interruccion 
}

/*Sensor UltraSonido e interruccion*/
void loop()//funcion loop
{
  digitalWrite(OUT,LOW);//apaga la variable del pin del sensor ultra
  delayMicroseconds(5);//hace una espera de 5 microsegundos
  digitalWrite(OUT, HIGH);//enciende la variabe del pin del sensor ultra
  delayMicroseconds(10);//hace una espera de 10 microsegundos
  tiempo=pulseIn(IN, HIGH);
  distancia= (tiempo/2)/29; //conversion de la disteancia en cm
  if(distancia <=20 && distancia>=2) { //distancia a la qe se ejecutaran las funciones de iterruccion y donde entra el sensor ultra
    
   /*Frena el carrito*/    //Parar los motores por 500 mili segundos
    digitalWrite(13, LOW);//apaga el pin
    digitalWrite(12, LOW);//apaga el pin
    digitalWrite(9, LOW);//apaga el pin
    digitalWrite(8, LOW);//apaga el pin
        delay(500);//tiempo de espera
        
    /*Retrocede el Carrito*////Reversa durante 1000 mili segundos
    digitalWrite(13, revoluciones);//pasa la velocidad a los motores
    digitalWrite(12, LOW);//apaga el pin
    digitalWrite(9, revoluciones);//pasa la velocidad a los motores
    digitalWrite(8,LOW);//apaga el pin
        delay(800);//timepo qe retrocedera
    
    // Izquierda          //Girar durante 1100 milisegundos
    digitalWrite(13,revoluciones);//pasa la velocidad a los motores
    digitalWrite(12,LOW);//apaga el pin
    digitalWrite(9,LOW);//apaga el pin
    analogWrite(8, revoluciones);//pasa la velocidad a los motores
    delay(1100);//tiempo qe durara el giro
    
    //adelante
    digitalWrite(13, LOW);//apaga el pin
    digitalWrite(12, LOW);//apaga el pin
    digitalWrite(9,LOW);//apaga el pin
    digitalWrite(8, LOW);//apaga el pin

  }
  Serial.print("Distancia  ");//imprime "Distancia"
  Serial.print(distancia);// imprime el valor de la distancia
  Serial.println(" cm");//imprime cm
  delay(1000);//espera de 1000 milisegundos
}

/*Parte delcontrol remoto*/
void control()//funcion control
{
  if (irrecv.decode(&results)) { //Intentará recibir un código de IR. Devuelve true si se ha recibido un código, o falso si no se ha recibido aún. Cuando se recibe un código, la información se almacena en "Resultados".
    Serial.println(results.value );//impreme el valor de la señal en el monitor serial.
    irrecv.resume();//Después de recibir, esto se debe llamar al reiniciar el receptor y prepararla para recibir otro código.
    digitalWrite(4,HIGH);//activamos el pin 4 para qe el led encienda cuando aya una interferencia departe del IR emisor.
    delay(20);//hace una espera de 20 milisegundos de parpadeo
    digitalWrite(4,LOW);//apaga el led cuando no aya interruccion del IR emisor
  }
    if(results.value == 284106975)//Detener/Freno - el código actual IR (0 si el tipo es desconocido) 
    {
    digitalWrite(13,LOW);//apaga el pin
    digitalWrite(12,LOW);//apaga el pin
    digitalWrite(9,LOW);//apaga el pin
    digitalWrite(8,LOW);//apaga el pin
    }
    
    if(results.value == 284139615)//Adelante
    {
    digitalWrite(13, LOW);//apaga el pin
    analogWrite(12, revoluciones);//da paso a las revoluciones del motor
    digitalWrite(9,LOW);//apaga el pin
    analogWrite(8,revoluciones);//da paso a las revoluciones del motor
    }
    
    if(results.value == 284098815)//Atras
    {
    analogWrite(13, revoluciones);//da paso a las revoluciones del motor
    digitalWrite(12, LOW);//apaga el pin
    analogWrite(9,revoluciones);//da paso a las revoluciones del motor
    digitalWrite(8,LOW);  //apaga el pin
    }
    if(results.value == 284102895)//Izquierda
    {
    digitalWrite(13,revoluciones);//da paso a las revoluciones del motor
    digitalWrite(12,LOW);//apaga el pin
    digitalWrite(9,LOW);//apaga el pin
    analogWrite(8, revoluciones);//da paso a las revoluciones del motor
    }  
    
    if(results.value == 284131455)//Derecha
    {
    digitalWrite(13, LOW);//apaga el pin
    analogWrite(12, revoluciones);//da paso a las revoluciones del motor
    digitalWrite(9, revoluciones);//da paso a las revoluciones del motor
    digitalWrite(8, LOW);//apaga el pin
    }
}
