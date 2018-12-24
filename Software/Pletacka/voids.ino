//Void funkce//

/*
/////////////////vykresli displej
void DPinfo(void)
{
 
  switch (info)
  {
  case 'r':
    DPrun();

  case 'p':
    DPpaus();

  case 's':
    DPslow();
    
  }

}*/
/*///////////////
Special
///////////////*/
void TMrozdil(void)
  {
    poc= millis();
    
    switch (stav)
    {
      case JEDU:
        pocroz+= poc-pocmin; 
        pocmin=poc;
        break;

      case POMALU:
        pocroz+= ((poc-pocmin)/2); 
        pocmin=poc;
        break;
    }
    
    
    pocroz+= poc-pocmin; 
    pocmin=poc;
  }

void TMreset(void)
  {
    pocroz=0;
  }




/*///////////////
Motor
///////////////*/

/////////////////motor set speed
void Mset(int motorspeed)
{
  motor.reset();
  motor.setSpeed(motorspeed);
 
}

/*///////////////
Displej
///////////////*/

/////////////////vykresli displej
void DPdshow(void)
{
  display.display();
}

/////////////////smaz displej
void DPclear(void)
{
  display.clearDisplay();
}
/////////////////podklad pod displej
void DPdisp(void)
{

  display.drawRect(0,  0, 40, 32,  WHITE);  
  display.drawRect(40, 0, 96, 32,  WHITE);  
  display.drawRect(96, 0, 32, 32,  WHITE);  
   


}
/////////////////pravasipka
void DPpravasip(void)
{
  display.drawLine(104, 16, 120, 16, WHITE);
  display.drawLine(120, 16, 114, 10, WHITE);
  display.drawLine(120, 16, 114, 22, WHITE);
}

/////////////////levasipka
void DPlevasip(void)
{
  display.drawLine(104, 16, 120, 16, WHITE);  
  display.drawLine(104, 16, 110, 10, WHITE);
  display.drawLine(104, 16, 110, 22, WHITE);  
}

/////////////////displej run
void DPrun(void)
{
  display.setFont(&FreeSerif9pt7b);
  display.setTextSize(1);
  // Print text:
  display.setCursor(4, 20);  // (x,y)
  display.println("Run");  // Text or value to print
}

/////////////////displej paus
void DPpaus(void)
{
  display.setFont(&FreeSerif9pt7b);
  display.setTextSize(1);
  // Print text:
  display.setCursor(2, 20);  // (x,y)
  display.println("Paus");  // Text or value to print
}

/////////////////displej slow
void DPslow(void)
{
  display.setFont(&FreeSerif9pt7b);
  display.setTextSize(1);
  // Print text:
  display.setCursor(2, 20);  // (x,y)
  display.println("Slow");  // Text or value to print
}

/////////////////displej lytko
void DPlytko(void)
{
  display.setFont(&FreeSerif9pt7b);
  display.setTextSize(1);
  // Print text:
  display.setCursor(42, 20);  // (x,y)
  display.println("Lytko");  // Text or value to print
}

/////////////////displej pata
void DPpata(void)
{
  display.setFont(&FreeSerif9pt7b);
  display.setTextSize(1);
  // Print text:
  display.setCursor(42, 20);  // (x,y)
  display.println("Pata");  // Text or value to print
}

/////////////////displej nart
void DPnart(void)
{
  display.setFont(&FreeSerif9pt7b);
  display.setTextSize(1);
  // Print text:
  display.setCursor(42, 20);  // (x,y)
  display.println("Nart");  // Text or value to print
}

/////////////////displej spice
void DPspice(void)
{
  display.setFont(&FreeSerif9pt7b);
  display.setTextSize(1);
  // Print text:
  display.setCursor(42, 20);  // (x,y)
  display.println("Spice");  // Text or value to print
}

/////////////////pauza, vypadek ponozky
void DPponoz(void)
{
  display.fillRoundRect(106, 6, 11, 15,0,WHITE );
  display.drawLine(106, 21, 116, 27, WHITE);
}


/////////////////start
void DPange(void)
{

  

  //Cisteni displeje
  display.clearDisplay();
  motor.stop();
      
  //cele modre
  display.fillRoundRect(0, 0, 128, 32, 0, WHITE);
  //reset
  display.display();
  display.clearDisplay();
  delay(2000);
  //nastaveni fontu
  display.setFont(&FreeSans12pt7b);
  display.setTextSize(1);
  //vypis
  display.setCursor(4, 25);  // (x,y)
  display.println("ANGE 19K");  // Text or value to print
  
  display.display();
  display.clearDisplay();
  delay(4000);
  
  display.setCursor(4, 25);  // (x,y)
  display.println("OD KUBY");  // Text or value to print

  display.display();
  display.clearDisplay();
  delay(4000);
  display.display();
}

void DPload(void)
{  

  display.setCursor(4, 25);  // (x,y)
  display.println("Loading");  // Text or value to print

  display.display();
  display.clearDisplay();
  delay(400);


  for (int load = 0; load<3;load++)
  {
    display.setCursor(4, 25);  // (x,y)
    display.println("Loading");  // Text or value to print
    digitalWrite(L_red, HIGH);
    
    display.display();
    display.clearDisplay();
    delay(300);

    digitalWrite(L_red, LOW);
    digitalWrite(L_slow, HIGH);         
    display.setCursor(4, 25);  // (x,y)
    display.println("Loading.");  // Text or value to print

    display.display();
    display.clearDisplay();
    delay(300);

    digitalWrite(L_slow, LOW);
    digitalWrite(L_green, HIGH);
    display.setCursor(4, 25);  // (x,y)
    display.println("Loading..");  // Text or value to print

    display.display();
    display.clearDisplay();
    delay(300);

    digitalWrite(L_green, LOW);                  
    display.setCursor(4, 25);  // (x,y)
    display.println("Loading...");  // Text or value to print

    display.display();   
    display.clearDisplay();
    delay(300);
    prvni=5;
  }
  display.display();
  
}



