#define RESET_PIN 2
char packet[ ] = "$GPGDA,033410.000,2232.1745,N,11401.1920,E,1,07,1.1,107.14,M,0.00,M,,*64\r\n$GPGGA,033410.000,2232.1745,N,11401.1920,E,1,07,1.1,107.14,M,0.00,M,,*64\r\n";


char key[] = "GPGGA";

byte counter=-1;
float lt=0;//Keep the format of the input
float lg=0;//Keep the format of the input
int alt=0;//Keep the format of the input  
byte nSatelites = 0;
char ltInd = 'S';
char lgInd = 'W';
char altUnits = 'M';
byte tokenIndex = 0;
float multLt = 0;
float multLg = 0;
byte altDecimal = 0;


// the setup routine runs once when you press reset:
void setup() {                
  Serial.begin(115200);
  Serial1.begin(9600);

  pinMode(14, OUTPUT); // just a led to check if it works

  pinMode(RESET_PIN, OUTPUT); // we put the reset to pin 9
  digitalWrite(RESET_PIN,HIGH);
  delay(300);
  digitalWrite(RESET_PIN,LOW);
  delay(300);
  digitalWrite(RESET_PIN,HIGH);
}

// the loop routine runs over and over again forever:
void loop() {
  //Serial.print("Here");
  char input = 0;
  if (Serial1.available()) {
    input = (char)Serial1.read();  

  //for (byte i=0; i<sizeof(packet); i++) {
  //  input = packet[i];
    switch(input){
    case '$':
      //Sent package and start a again
      counter=0;
      tokenIndex=0;
      lt = 0;
      lg = 0;
      nSatelites = 0;
      multLt = 1000;
      multLg = 10000;
      alt = 0;
      altDecimal = 0;
      break;
    case ',':
      if(counter>=0)
        counter++;
      break;
    case '\r':
    break;
    case '\n':
      if (counter>11&&nSatelites>0&&nSatelites<13) {
        int lgInt = (int)lg;
        int lgDec = (lg-lgInt)*10000;
        if(lgInd=='S'){
          lgInt*=-1;
        }
        int ltInt = (int)lt;
        int ltDec = (lt-ltInt)*10000;
        if(ltInd=='W'){
          ltInt*=-1;
        }
        
        Serial.print(" lt: ");
        Serial.print(lt);
        Serial.print(" ");
        Serial.print(ltInt);
        Serial.print(" ");
        Serial.print(ltDec);
        Serial.print(" lg: ");
        Serial.print(lg);
        Serial.print(" ");
        Serial.print(lgInt);
        Serial.print(" ");
        Serial.print(lgDec);
        Serial.print(" alt: ");
        Serial.print(alt);
        Serial.print(" altUnits: ");
        Serial.print(altUnits);
        Serial.print(" nSat: ");
        Serial.print(nSatelites);
        Serial.print(" ");
        Serial.print("\r\n");
      }
      counter=-1;//We need to wait for the next dollar mark to start again
      break;
    default:
      switch(counter){
        case 0:
          if (tokenIndex>4||key[tokenIndex]!=input) {
            counter=-1; 
          }
          tokenIndex++;
          //if (tokenIndex==5) {
          //  counter=1;
          //}
          break;
        case 2://Latitude
          if(input>47&&input<58){
            lt+=(input-48)*multLt;
            multLt/=10;
          }
          break;
        case 3://Latitude indicator
          ltInd=input;
          break;
        case 4://Longitude
          if(input>47&&input<58){
            lg+=(input-48)*multLg;
            multLg/=10;
          }
          break;
        case 5://Longitude indicator
          lgInd=input;
          break;
        case 7://Longitude indicator
          if(input>47&&input<58){
            nSatelites=nSatelites*10+(input-48);
          }
          break;
        case 9:
          if(input>47&&input<58){
            alt=alt*10+(input-48);
            //altDecimal*=10;
          }
          else{
            counter=10;
          }
          break;
        case 10:
          break;
        case 11:
          altUnits=input;
          break;
      }
      break;
    }
    //}
  }

  delay(3);
}

