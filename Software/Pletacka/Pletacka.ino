
/*/////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
Model pletacky - program
21.12.2018
By Jakub Andrysek

Zacatek tvorby 2018-12-26
Pletacka dokoncena 2018-12-22
Go to: https://github.com/JakubAndrysek/Model_pletacky_na_ponozky
///////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////*/


////Include
//Knihovny
#include <L298N.h>
#include <ButtonDebounce.h>
#include <Adafruit_SSD1306.h>  // Include Adafruit_SSD1306 library to drive the display
Adafruit_SSD1306 display;  // Create display

//Fonty
//https://learn.adafruit.com/adafruit-gfx-graphics-library/using-fonts
#include <Fonts/FreeSans12pt7b.h>   //vetsi pismo
#include <Fonts/FreeSerif9pt7b.h>   //mensi pismo

///////////////////////////
////Definovani
//Motor
#define EN 5
#define IN1 6
#define IN2 7
L298N motor(EN, IN1, IN2);
//unsigned short theSpeed = 255;
////////////////////////////

ButtonDebounce BT_start(2, 100);
ButtonDebounce BT_stop(3, 100);
ButtonDebounce BT_slow(4, 100);

////Promenne

//Motor rychly 
int T_lytko = 5000;
int T_pata = 500;
int Pocet_pata = 7;
int T_nart = 5000;
int T_spice = 500;
int Pocet_spice = 7;
int T_stop = 4000;

int Pocet_pata_now =0;
int Pocet_spice_now =0;

int pocitadlo = 0;

//Led
int L_green = 8;
int L_red = 10;
int L_slow = 9;
int L_orange = 11;
int L_orange_bright = 35;

//Pocitadlo
unsigned long poc=0;//pocitadlo
unsigned long pocmin=0;//pocitadlo minule
unsigned long pocroz= 0;//pocitadlo rozdil

//Dalsi
int prvni = 1;//Pro Ange okno
bool db = true; //Debugovaci promena

////////////Enumy
//Krok stroje
enum tKrok
{
  LYTKO,
  PATA,
  NART,
  SPICE,
  PAUZA
}krok=LYTKO;

//Stav stroje
enum tStav
{
  JEDU,
  STOJIM,
  CHYBA,
  POMALU
}stav=JEDU;

enum tSmer
{
  DOPRAVA,
  DOLEVA
}smer=DOLEVA;

void setup() {
  delay(100);//Kvuli OLED
  
  Serial.begin(9600);
  

  ////Inicializace
  //rychlost motoru
  motor.setSpeed(255);


  //init ledek  
  pinMode(L_green, OUTPUT);
  pinMode(L_red, OUTPUT);
  pinMode(L_slow, OUTPUT);
  pinMode(L_orange, OUTPUT);
  
  //OLED
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);   
 
  display.clearDisplay();  // Vycisti displej

  display.setTextColor(WHITE);  // Nastaveni barvy
  display.setRotation(0);  // Orientace displeje - 0, 1, 2 or 3
  display.setTextWrap(false);  //Automaticke radkovani
  
  display.dim(0);  //Nastaveni jasu (0 is maximun and 1 is a little dim)


  ////      ----> forward
  ////      <---- backward

  DPange();
  DPload();

}

void loop() {
  //Aktualizace stavu tlacitek
  BT_start.update();
  BT_stop.update();
  BT_slow.update();


  if(pocitadlo == 3)
  {
    if(db)
    Serial.println("Ange");
    DPange();//startovaci okno
    pocitadlo=0;
      
  }

  Serial.println(pocitadlo);

  //Vypis na displej
  DPclear(); //Vymaze displej
  DPdisp(); //Vykresli ramecky  

  
  //Pomalu
  if((BT_slow.state() == LOW)and((krok!=PAUZA)and(stav!=STOJIM)))
  {

    Mset(180);//Nastaveni rychlosti motoru
    stav=POMALU;//Zmena stavu
    digitalWrite(L_green, LOW);
    digitalWrite(L_slow, HIGH);
    analogWrite(L_orange, L_orange_bright);   
 
  }
  else
  {    
    //Jedu
    if(BT_start.state() == LOW)
    {
      Mset(255);//Nastaveni rychlosti motoru
      stav=JEDU;//Zmena stavu
      digitalWrite(L_green, LOW);
      digitalWrite(L_orange, LOW);
      digitalWrite(L_slow, LOW);
    }
  
    //Stojim
    else if(BT_stop.state() == LOW)
    {
      Mset(255);//Nastaveni rychlosti motoru
      stav=STOJIM;//Zmena stavu
      digitalWrite(L_green, HIGH);
      analogWrite(L_orange, L_orange_bright);
      digitalWrite(L_slow, LOW);
    }
    else if(stav==POMALU)
    {
      Mset(255);//Nastaveni rychlosti motoru
      stav=JEDU;//Zmena stavu
      digitalWrite(L_green, LOW);
      digitalWrite(L_orange, LOW);
      digitalWrite(L_slow, LOW);
    }
  }




   if(db)
   Serial.println("Motor");
  switch (krok) 
  {
    //LYTKO//////////////////////////
    case LYTKO:
       DPpravasip(); //vykresli sipku
       DPlytko(); //Napise...
      //Svav pletacky
      switch (stav)
      {
       case POMALU:
          DPslow();//Napise...   
                        
        case JEDU:
          TMrozdil(); //Timer        
          if (stav==JEDU){DPrun();}  //Napise...  
          if (pocroz<T_lytko) //Zjisteni casu
          {
            motor.forward(); //Smer motoru
          }
          else
          {
            motor.stop(); //Zastaveni motoru //Zastaveni motoru
            TMreset(); //Reset timeru
            krok=PATA; //Prechod na dalsi krok
          }
          break;
                    
        case STOJIM: 
          motor.stop(); //Zastaveni motoru
          DPpaus();  //Napise...
          break;
          
        case CHYBA:
          break;


          
      }
       
      break;
 
  //PATA//////////////////////////

  case PATA:
    DPpata(); //Napise...
    //Svav pletacky
    switch (stav)
    {
      case POMALU:                         
        DPslow();//Napise...               
      
      
      case JEDU:
        TMrozdil(); //Timer        
        if (stav==JEDU){DPrun();}  //Napise...
        if (pocroz<T_pata) //Zjisteni casu
        {
          switch (smer)//Smer toceni
          {
            case DOLEVA:
              motor.backward(); //Smer motoru                           
              break;
              

            case DOPRAVA:
              motor.forward(); //Smer motoru
              DPpravasip(); //vykresli sipku
              break;                           
          }
        }
        else
        {
          motor.stop(); //Zastaveni motoru //Zastaveni motoru
          TMreset(); //Reset timeru
          if (smer==DOLEVA)//Zmena smeru toceni
          {
            smer = DOPRAVA;          
          }
          else
          {
            smer = DOLEVA;      
          }
          

          
          if (Pocet_pata_now<Pocet_pata)//Kontrola poctu zmen
          {
            krok=PATA; //Zpet na PATA
            Pocet_pata_now++; //Zvuseni hodnoty
          }
          else
          {
            krok=NART; //Prechod na dalsi krok
            Pocet_pata_now=0; //Vyresetovani pocitadla zmen
          }
      
        }        
        break;
        
        
      case STOJIM: 
        motor.stop(); //Zastaveni motoru
        DPpaus();  //Napise...   
        break;
        
      case CHYBA:
        break;

        
        
    }
    
    switch(smer)
    {
      case DOLEVA:
        DPlevasip(); //vykresli sipku
        break;
        
      case DOPRAVA:
        DPpravasip(); //vykresli sipku
        break;
    }
          
    break;


  //NART//////////////////////////
  case NART:
    DPpravasip(); //vykresli sipku
    DPnart(); //Napise...   
    //Svav pletacky
    switch (stav)
    {
      
      case POMALU:
        DPslow();//Napise... 
              
      case JEDU:
        TMrozdil(); //Timer        
        if (stav==JEDU){DPrun();}  //Napise...    
        if (pocroz<T_nart) //Zjisteni casu
        {
          motor.forward(); //Smer motoru
        }
        else
        {
          motor.stop(); //Zastaveni motoru //Zastaveni motoru
          TMreset(); //Reset timeru
          krok=SPICE; //Prechod na dalsi krok
        }
        break;
        
      case STOJIM: 
        motor.stop(); //Zastaveni motoru
        DPpaus();  //Napise...
        break;
        
      case CHYBA:
        break;

   
  
        
    }
     
    break;

  //SPICE//////////////////////////
  case SPICE:
    DPspice(); //Napise...
    //Svav pletacky
    switch (stav)
    {
      
      case POMALU:     
        DPslow();//Napise...        
              
      case JEDU:
        TMrozdil(); //Timer        
        if (stav==JEDU){DPrun();}  //Napise...  
        if (pocroz<T_spice) //Zjisteni casu
        {
          switch (smer)//Smer toceni
          {
            case DOLEVA:
              motor.backward(); //Smer motoru
              DPlevasip(); //vykresli sipku             
              break;
              

            case DOPRAVA:
              motor.forward(); //Smer motoru
              DPpravasip(); //vykresli sipku
              break;                           
          }
        }
        else
        {
          motor.stop(); //Zastaveni motoru //Zastaveni motoru
          TMreset(); //Reset timeru
          if (smer==DOLEVA)//Zmena smeru toceni
          {
            smer = DOPRAVA;          
          }
          else
          {
            smer = DOLEVA;      
          }
          

          
          if (Pocet_spice_now<Pocet_spice)//Kontrola poctu zmen
          {
            krok=SPICE; //Zpet na SPICE
            Pocet_spice_now++; //Zvuseni hodnoty
          }
          else
          {
            krok=PAUZA; //Prechod na dalsi krok
            Pocet_spice_now=0; //Vyresetovani pocitadla zmen
            
 
          }
      
        }
        break;
        
      case STOJIM: 
        motor.stop(); //Zastaveni motoru
        DPpaus();  //Napise...       
        break;
        
      case CHYBA:
        break;

 
            
        
    }
    //Vykresleni spravne sipku
    switch(smer)
    {
      case DOLEVA:
        DPlevasip(); //vykresli sipku
        break;
        
      case DOPRAVA:
        DPpravasip(); //vykresli sipku
        break;
    }     
    break;

  //PAUZA//////////////////////////
  case PAUZA:
    //Svav pletacky
    DPpaus();
    DPponoz();
    switch (stav)
    {
      case POMALU:           
      case JEDU:
        digitalWrite(L_slow, LOW);     
        digitalWrite(L_orange, LOW); 
        TMrozdil();         
        if (pocroz<T_stop)
        {
          motor.stop(); //Zastaveni motoru
        }
        else
        {
          TMreset();
          krok=LYTKO;
          pocitadlo++;
          if (db)
          Serial.println("Pocitadlo++");
          
        }
        break;
     case STOJIM: 
        motor.stop(); //Zastaveni motoru
        break;
        
        
    }
     
    break;

  

  }//Swich krok
  
  //Displej
  DPdshow(); //Vykresli na displej
  
}//Loop

