/*
 ========================================                ======================================
                                          HOME AUTOMATION
 ========================================                ======================================

 =============================================
 autor: Rujan Andrei
 ----------------
 tipul lucrarii: Lucrare de licenta
 ----------------
 facultatea: Automatica si Calculatoare
 ----------------
 specializarea: Ingineria Sistemelor
 ----------------
 =============================================
 */

 /* includerea bibliotecilor necesare bibliotecilor necesare */
 //#include <WiFi.h>
 #include <Wire.h>
 //#include <WebServer.h>
 #include <DHT.h>
 #include <Servo.h>

/*
 ===================================================================================
                                 --DECLARATII--
 ===================================================================================
 */

 /*----------------------------------TEMPERATURA----------------------------------*/
 /* definirea pinilor pentru senzorului de temperatura si ledului corespunzator */

 #define tempumiSensorPin 2
 #define templed 12 // pinul pe care este setat ledul aferent senzorului de temperatura

 /* definirea intervalului min/max pentru temperatura din camera */

 #define mintemp 20
 #define maxtemp 23

 /*--------------------------------------GAZ--------------------------------------*/
 /* definirea pinilor pentru senzorului de gaz si ledului corespunzator */

 #define gasSensorPin  A0 // Pinul analogic pe care este conectat senzorul de gaz
 #define gasled 13 // pinul pe care este setat ledul aferent senzorului de gaz

 /* definirea unui prag pentru gazul din casa, care odata depasit devine periculos */

 #define maxgas 200

 /*-----------------------------------UMIDITATE-----------------------------------*/
 /* definirea pinilor pentru senzorului de umiditate si ledului corespunzator */

 //#define tempumiSensorPin 2
 #define umiled 11 // pinul pe care este setat ledul aferent senzorului de umiditate

 /* definirea intervalului min/max pentru umiditatea din camera */

 #define minumi 40
 #define maxumi 80

 /*----------------------------------SERVO MOTOR----------------------------------*/
 /* definirea pinului pentru servo motor */

 #define servoPin A1

 /* instantierea unui obiect de tip "Servo", aferent servo motorului */

 Servo motor;
 #define std_on  100
 #define std_off 0
 /*--------------------------------------DHT--------------------------------------*/
 #define DHTTYPE DHT11
 DHT dht(2, DHTTYPE); //instantierea unui obiect de tip DHT

/*---------------------------------------I2C---------------------------------------*/
 #define I2C_SLAVE_ADDR 0x08

 /*-------------------------------------------------------------------------------*/

void setup() {
  Serial.begin(115200);
  Wire.begin();
  dht.begin();
  motor.attach(servoPin);
  motor.write(std_off);
  pinMode(templed,OUTPUT);
  pinMode(gasled,OUTPUT);
  pinMode(umiled,OUTPUT);
}

/*==================================================================================*/
class Temperatura{
     public:
     float actualtemp;

     /*Prototipurile functiilor folosita in clasa temperatura*/
     // float CitireTemperaturaSenzor();
     // void AfisareTemperatura(void);
     // int VerificareTemperatura(void);

     Temperatura(float actualtemp)
     {
         this->actualtemp=actualtemp;
     }

     float CitireTemperaturaSenzor()
     {
         actualtemp = dht.readTemperature();         //primirea datelor de la senzor 
        
         return actualtemp;
     }

     void AfisareTemperatura(void)
     {
         actualtemp=CitireTemperaturaSenzor();

         Serial.print("Temperatura din camera este: ");
         Serial.print(actualtemp);
         Serial.print("\n");
     }

     int VerificareTemperatura(void)
     {
         int ok=1;// ok=0=> totul este in parametrii
         actualtemp=CitireTemperaturaSenzor();
         if(actualtemp< mintemp || actualtemp> maxtemp)
         {
             //aprinderea ledului care semnaleaza ca temperatura nu e in limite
             digitalWrite(templed, HIGH);
             Serial.print("Temperatura din incapere este");
             Serial.println(actualtemp);
             ok=1;
         }
         else
         {
             //ledul ramame stins
             digitalWrite(templed, LOW);
             Serial.println("Temperatura din incapere este");
             Serial.println(actualtemp);
             ok=0;
         }
         return ok;
     }

 };

 class Gaz{
     public:
     float actualgas;

     /*Prototipurile functiilor folosita in clasa gazul*/
     // float CitireGazSenzor();
     // void AfisareGas(void);
     // int VerificareGas();

     Gaz(float actualgas)
     {
         this->actualgas= actualgas;
     }

     float CitireGazSenzor()
     {
         actualgas = analogRead(gasSensorPin); // Citirea valorii analogice de la senzor

         return actualgas;
     }

     void AfisareGas(void)
     {
         actualgas=CitireGazSenzor();
         Serial.print("Valoarea gazului din camera este: ");
         Serial.print(actualgas);
         Serial.print("\n");
     }

     int VerificareGas()
     {
         int ok=1;// ok=0=> totul este in parametrii
         actualgas=CitireGazSenzor();
         if(actualgas> maxgas)
         {
             Serial.print("\nG--EAMURI DESCHISE--\n");
             Serial.print("Valoare gazului a depasit limita!! Aceasta este: ");
             Serial.println(actualgas);
             Serial.print("\n");
             digitalWrite(gasled, HIGH);
             motor.write(std_on);
             ok=1;
         }
         else
         {
             Serial.print("\n--GEAMURI INCHISE--\n");
             Serial.print("Valoare gazului este in limita!! Aceasta este: ");
             Serial.println(actualgas);
             Serial.print("\n");
             digitalWrite(gasled, LOW);
             motor.write(std_off);
             ok=0;
         }
         return ok;
     }

 };

 class Umiditate{
     public:
     float actualumidity;

     /*Prototipurile functiilor folosita in clasa Umiditate*/
     // void CitireUmiditateSenzor();
     // void AfisareUmiditate();
     // int VerificareUmiditate();

     Umiditate(float actualumidity){
         this->actualumidity=actualumidity;
     }

     float CitireUmiditateSenzor(){
         actualumidity = dht.readHumidity(); //citirea valorii analogice de la senzor        

         return actualumidity;
     }

     void AfisareUmiditate(){
         actualumidity=CitireUmiditateSenzor();
         Serial.print("Nivelul de umiditate din camera este din camera este: ");
         Serial.print(actualumidity);
         Serial.print("\n");
     }

     int VerificareUmiditate(){
        int ok=1;// ok=0=> totul este in parametrii
        if(actualumidity<minumi || actualumidity>maxumi){
          Serial.print("Nivelul de umiditate din camera nu este in parametrii");
          Serial.print("\n");
          digitalWrite(umiled, HIGH);
          ok=1;
        }
        else{
          Serial.print("Nivelul de umiditate din camera este in parametrii");
          Serial.print("\n");
          digitalWrite(umiled, LOW);
          ok=0;
        }
        return ok;
     }

 };
/*==================================================================================*/

/*Functii aditionale*/
/*==================================================================================*/
 void Verificare(int stare, String senzor, int led){
  if(stare){
    digitalWrite(led, HIGH);
    if(senzor == "gaz"){
      motor.write(std_on);
    }
  }
  else{
    digitalWrite(led, LOW);
    if(senzor == "gaz"){
      motor.write(std_off);
    }
  }
 }

 void DelimitareAfisare(){
  int i=0;
  for(i=0;i<500;i+=100){
    Serial.print(". ");
    delay(100);
  }
 }
/*==================================================================================*/

void loop() {
  float temperatura;
  float umiditatea;
  float gazul;

  int status=1;

  Temperatura temp(temperatura);
  Umiditate umi(umiditatea);
  Gaz gaz(gazul);

  temperatura= temp.CitireTemperaturaSenzor();
  umiditatea=  umi.CitireUmiditateSenzor();
  gazul=       gaz.CitireGazSenzor();

  /*Afisari locale*/
  Serial.println("***************************");
  
  status= temp.VerificareTemperatura();
  Verificare(status, "temperatura", templed);
  //DelimitareAfisare();

  status= umi.VerificareUmiditate();
  Verificare(status, "umiditate", umiled);
  //DelimitareAfisare();

  status= gaz.VerificareGas();
  Verificare(status, "gaz", gasled);
  //DelimitareAfisare();

  Serial.println("***************************");
  delay(1000);

  if (isnan(umiditatea) || isnan(temperatura) || isnan(gazul)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  Wire.beginTransmission(I2C_SLAVE_ADDR);
  Wire.write((uint8_t*)&temperatura, sizeof(temperatura));
  Wire.write((uint8_t*)&umiditatea, sizeof(umiditatea));
  Wire.write((uint8_t*)&gazul, sizeof(gazul));
  Wire.endTransmission();

  Serial.print("Sent Temperature: ");
  Serial.print(temperatura);
  Serial.print(" Humidity: ");
  Serial.println(umiditatea);
  Serial.print(" Gas: ");
  Serial.println(gazul);

  delay(2000);
}