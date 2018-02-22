
#include <AFMotor.h>

AF_DCMotor motor1(1), motor2(4);

int sensor0 = 0;
int sensor1 = 0;
int sensor2 = 0;
int sensor3 = 0;
int sensor4 = 0;
int sensor5 = 0;
int sensor6 = 0;
int sensor7 = 0;
int sensorFR =0;
int sensorFL =0;
int sensorLeft =0;
int sensorRight=0;

char memory[50];
int counter = 0;

int tracking=0; // O hobe

int crit = 500; // this needs to be tuned on the track. 


// this speed is for line follow.
int SpeedMax=120; //110 wrking

// PID gain for line follow. initially set zero.
float Kp =0;
float Kd =0;
float Ki =0;

// PID calculation for wall follow. 
int preverror = 0;
int error = 0;
int derror = 0;
int ierror = 0;
int U;
int led= 46;

  int rightLed = 41;
  int leftLed = 43;
  int sLed = 42;
  
int follow=0;
int notfollow=1;
float avg[10];
float total;
void setup() {
   Serial.begin(9600);  
         // set up Serial library at 9600 bps
         
  Serial.println("Motor test!");

  // turn on motor
  motor1.setSpeed(SpeedMax);
  motor1.run(RELEASE);
  motor2.setSpeed(SpeedMax);
  motor2.run(RELEASE);
  
   pinMode(led, OUTPUT);
   
   pinMode(41, OUTPUT);
   
   pinMode(42, OUTPUT);
   
   pinMode(43, OUTPUT);
   
  for (int i=0; i<10; i++)
  {
  senser();
  if (sensor0==1 && sensor7==1)
  {
    avg[i] = 0;
  }
  else avg[i] =1;
  }
  
  total = (avg[0]+avg[1]+avg[2]+avg[3]+avg[4]+avg[5]+avg[6]+avg[7]+avg[8]+avg[9])/10;
 if (total>0.5)
 {
   follow =1;
   notfollow=0;
 }
 else 
 {
   follow =0;
   notfollow=1;
 }
}


void loop() {
  
  senser();
  printPath();
  
  if (tracking == 0)
  {  
    digitalWrite (rightLed,LOW);
    digitalWrite (leftLed,LOW);
    digitalWrite (sLed,LOW);
    digitalWrite (led,LOW);
    
   
  if (sensor0!=follow && sensor1 != follow && sensor2!=follow && sensor3 != follow && sensor4!=follow && sensor5 != follow && sensor6!=follow && sensor7 != follow  && sensorRight!=follow && sensorLeft!=follow)
  {
   
   memory[counter]='U';
   digitalWrite (led,HIGH);
   //delay(300);
   while(sensor2!=follow && sensor3!=follow && sensor4!=follow && sensor1!=follow)
   {
    turnU();
    senser();
   }
   
   counter++;
    pathShort();
  }
   
    // Turn left priority and save memory
    else if(sensorLeft==follow && sensorRight==follow)
    {
      digitalWrite (leftLed,HIGH);
      memory[counter]='L';
      while(sensorLeft==follow)
    {  
      
      turnLeft();
      delay(400); // last editied 
      senser();
    }
    counter++;
    
    pathShort();
    
  }
   
    
    else if (sensorLeft==follow  && (sensor3 == follow || sensor4==follow))
    {
      digitalWrite (leftLed,HIGH);
      memory[counter]='L';
      while(sensorLeft==follow)
    {  
      
      turnLeft();
      if(sensor0==follow&& sensor1==follow&&sensor2==follow&&sensor3==follow&&sensor4==follow&&sensor5==follow&&sensor6==follow&&sensor7==follow && sensorRight==follow && sensorLeft==follow)
      {    
        digitalWrite (led,HIGH);
        Stop();
        delay(1000);
        digitalWrite(led,LOW);
        delay(1000);
        digitalWrite(led,HIGH);
        delay(1000);
        digitalWrite(led,LOW);
        delay(1000);
        digitalWrite(led,HIGH);
        delay(1000);
        digitalWrite(led,LOW);
        
        
        tracking=1;
        counter--;
        turnU();
        delay(2000);
        
        //code incomplete
      }
      delay(400); // to be adjustable. 
      senser();
    }
    memory[counter]='L';
    counter++;
     pathShort();
    }
    
  
  // Going Straight during intersection Condition
  else if(sensorLeft !=follow && sensorRight==follow && (sensor3 == follow && sensor4 == follow))
  {  
    delay(100);
    if(sensorLeft !=follow && sensorRight==follow && (sensor3 == follow && sensor4 == follow))
    {
    digitalWrite (sLed,HIGH);
    memory[counter]='S';
    while (sensorRight==follow)
    {
     goStraight();
     if(sensor0==follow&& sensor1==follow&&sensor2==follow&&sensor3==follow&&sensor4==follow&&sensor5==follow&&sensor6==follow&&sensor7==follow && sensorRight==follow && sensorLeft==follow)
      {    
        digitalWrite (led,HIGH);
        Stop();
        delay(1000);
        digitalWrite(led,LOW);
        delay(1000);
        digitalWrite(led,HIGH);
        delay(1000);
        digitalWrite(led,LOW);
        delay(1000);
        digitalWrite(led,HIGH);
        delay(1000);
        digitalWrite(led,LOW);
        
        counter--;
        tracking=1;
        turnU();
        delay(2000);
        
        //code incomplete
      }
     senser();
    }
    counter++;
     pathShort();
    }
    
  }
    
  // Turn left condition without intersection. 
  else if (sensorLeft==follow && sensorRight!=follow && sensor3!=follow && sensor4!=follow)
  {  
    //digitalWrite (leftLed,HIGH);
    while(sensor2!=follow && sensor3!=follow && sensor4!=follow && sensor5!=follow)
    { 
      turnLeft();
      //delay(300);
      senser();
    }
  }
  
  
  //Turn Right Condition
  else if (sensorRight==follow && sensorLeft!=follow && sensor3!=follow && sensor3!=follow)
  { 
    digitalWrite (rightLed,HIGH);
     while (sensor2!=follow && sensor3!=follow && sensor4!=follow && sensor5!=follow)
    {
      turnRight();
      senser();
    } 
    
  }
  
  
  
  
  
  
  // Keeping robot in track code
  else if(sensor1== follow || sensor2== follow || sensor3== follow || sensor4== follow || sensor5== follow || sensor6==follow)
  {
    SpeedMax=150;
    Kp = 10; //50
    Kd = 50; //100
    Ki = 0;
  }
  
  
  
  else 
  {
    Kp = 100;
    Kd = 0;
    Ki = 0;
  }
  
  }
  
  else if (tracking==1)
  {
    digitalWrite (rightLed,LOW);
    digitalWrite (leftLed,LOW);
    digitalWrite (sLed,LOW);
    digitalWrite (led,LOW);
    
   
  if (sensor0!=follow && sensor1 != follow && sensor2!=follow && sensor3 != follow && sensor4!=follow && sensor5 != follow && sensor6!=follow && sensor7 != follow  && sensorRight!=follow && sensorLeft!=follow)
  {
   
   memory[counter]='U';
   digitalWrite (led,HIGH);
   //delay(300);
   while(sensor2!=follow && sensor3!=follow && sensor4!=follow && sensor1!=follow)
   {
    turnU();
    senser();
   }
   
   counter++;
    pathShort();
  }
   
    // Turn left priority and save memory
    else if(sensorLeft==follow && sensorRight==follow)
    {
      digitalWrite (leftLed,HIGH);
      memory[counter]='L';
      while(sensorLeft==follow)
    {  
      
      turnLeft();
      delay(400); // last editied 
      senser();
    }
    counter++;
    
    pathShort();
    
  }
   
    
    else if (sensorLeft==follow  && (sensor3 == follow || sensor4==follow))
    {
      digitalWrite (leftLed,HIGH);
      memory[counter]='L';
      while(sensorLeft==follow)
    {  
      
      turnLeft();
      if(sensor0==follow&& sensor1==follow&&sensor2==follow&&sensor3==follow&&sensor4==follow&&sensor5==follow&&sensor6==follow&&sensor7==follow && sensorRight==follow && sensorLeft==follow)
      {    
        digitalWrite (led,HIGH);
        Stop();
        delay(1000);
        digitalWrite(led,LOW);
        delay(1000);
        digitalWrite(led,HIGH);
        delay(1000);
        digitalWrite(led,LOW);
        delay(1000);
        digitalWrite(led,HIGH);
        delay(1000);
        digitalWrite(led,LOW);
        
        
        tracking=1;
        counter--;
        turnU();
        delay(2000);
        
        //code incomplete
      }
      delay(400); // to be adjustable. 
      senser();
    }
    memory[counter]='L';
    counter++;
     pathShort();
    }
    
  
  // Going Straight during intersection Condition
  else if(sensorLeft !=follow && sensorRight==follow && (sensor3 == follow && sensor4 == follow))
  {  
    delay(100);
    if(sensorLeft !=follow && sensorRight==follow && (sensor3 == follow && sensor4 == follow))
    {
    digitalWrite (sLed,HIGH);
    memory[counter]='S';
    while (sensorRight==follow)
    {
     goStraight();
     if(sensor0==follow&& sensor1==follow&&sensor2==follow&&sensor3==follow&&sensor4==follow&&sensor5==follow&&sensor6==follow&&sensor7==follow && sensorRight==follow && sensorLeft==follow)
      {    
        digitalWrite (led,HIGH);
        Stop();
        delay(1000);
        digitalWrite(led,LOW);
        delay(1000);
        digitalWrite(led,HIGH);
        delay(1000);
        digitalWrite(led,LOW);
        delay(1000);
        digitalWrite(led,HIGH);
        delay(1000);
        digitalWrite(led,LOW);
        
        counter--;
        tracking=1;
        turnU();
        delay(2000);
        
        //code incomplete
      }
     senser();
    }
    counter++;
     pathShort();
    }
    
  }
    
  // Turn left condition without intersection. 
  else if (sensorLeft==follow && sensorRight!=follow && sensor3!=follow && sensor4!=follow)
  {  
    //digitalWrite (leftLed,HIGH);
    while(sensor2!=follow && sensor3!=follow && sensor4!=follow && sensor5!=follow)
    { 
      turnLeft();
      //delay(300);
      senser();
    }
  }
  
  
  //Turn Right Condition
  else if (sensorRight==follow && sensorLeft!=follow && sensor3!=follow && sensor3!=follow)
  { 
    digitalWrite (rightLed,HIGH);
     while (sensor2!=follow && sensor3!=follow && sensor4!=follow && sensor5!=follow)
    {
      turnRight();
      senser();
    } 
    
  }
  
  
  
  
  
  
  // Keeping robot in track code
  else if(sensor1== follow || sensor2== follow || sensor3== follow || sensor4== follow || sensor5== follow || sensor6==follow)
  {
    SpeedMax=150;
    Kp = 10; //50
    Kd = 50; //100
    Ki = 0;
  }
  
  
  
  else 
  {
    Kp = 100;
    Kd = 0;
    Ki = 0;
  }
  
  }
  
  else if(tracking == 3)
  {
    int memory[]={'R','S','R','R','S','L'};
    counter=6;
    if (sensor0==follow && sensor1==follow && sensor2==follow && sensor3==follow && sensor4==follow && sensor5==follow && sensor6==follow && sensor7==follow && sensorLeft==follow && sensorRight==follow)
    {  
      delay(100);
      if(sensor0==follow && sensor1==follow && sensor2==follow && sensor3==follow && sensor4==follow && sensor5==follow && sensor6==follow && sensor7==follow && sensorLeft==follow && sensorRight==follow)
   {
     digitalWrite (led,HIGH);
     Stop();
     delay(1000);
     digitalWrite (led,LOW);
     delay(1000);
     digitalWrite (led,HIGH);
     delay(1000);
     digitalWrite (led,LOW);
     delay(1000);
     digitalWrite (led,HIGH);
     delay(1000);
     digitalWrite (led,LOW);
     
     delay(100000);
   } 
   }
   
   else if(counter==0)
   {
        
    digitalWrite (rightLed,LOW);
    digitalWrite (leftLed,LOW);
    digitalWrite (sLed,LOW);
    digitalWrite (led,LOW);
    
   
  if (sensor0!=follow && sensor1 != follow && sensor2!=follow && sensor3 != follow && sensor4!=follow && sensor5 != follow && sensor6!=follow && sensor7 != follow  && sensorRight!=follow && sensorLeft!=follow)
  {
   
   memory[counter]='U';
   digitalWrite (led,HIGH);
   //delay(300);
   while(sensor2!=follow && sensor3!=follow && sensor4!=follow && sensor1!=follow)
   {
    turnU();
    senser();
   }
   
   counter++;
    pathShort();
  }
   
    // Turn left priority and save memory
    else if(sensorLeft==follow && sensorRight==follow)
    {
      digitalWrite (leftLed,HIGH);
      memory[counter]='L';
      while(sensorLeft==follow)
    {  
      
      turnLeft();
      //delay(300);
      senser();
    }
    counter++;
    
    pathShort();
    
  }
   
    
    else if (sensorLeft==follow  && (sensor3 == follow || sensor4==follow))
    {
      digitalWrite (leftLed,HIGH);
      memory[counter]='L';
      while(sensorLeft==follow)
    {  
      
      turnLeft();
      if(sensor0==follow&& sensor1==follow&&sensor2==follow&&sensor3==follow&&sensor4==follow&&sensor5==follow&&sensor6==follow&&sensor7==follow && sensorRight==follow && sensorLeft==follow)
      {    
        digitalWrite (led,HIGH);
        Stop();
        delay(3000);
        
        tracking=1;
        while (sensor0 == notfollow && sensor7 == notfollow)
        {
          turnU();
          senser();
        }
        //code incomplete
      }
      delay(400); // to be adjustable. 
      senser();
    }
    memory[counter]='L';
    counter++;
     pathShort();
    }
    
  
  // Going Straight during intersection Condition
  else if(sensorLeft !=follow && sensorRight==follow && (sensor3 == follow && sensor4 == follow))
  {  
    delay(100);
    if(sensorLeft !=follow && sensorRight==follow && (sensor3 == follow && sensor4 == follow))
    {
    digitalWrite (sLed,HIGH);
    memory[counter]='S';
    while (sensorRight==follow)
    {
     goStraight();
     if(sensor0==follow&& sensor1==follow&&sensor2==follow&&sensor3==follow&&sensor4==follow&&sensor5==follow&&sensor6==follow&&sensor7==follow && sensorRight==follow && sensorLeft==follow)
      {    
        digitalWrite (led,HIGH);
        Stop();
        delay(3000);
        tracking=1;
        while (sensor0 == follow && sensor7 !=follow)
        {
          turnU();
          senser();
        }
        //code incomplete
      }
     senser();
    }
    counter++;
     pathShort();
    }
    
  }
    
  // Turn left condition without intersection. 
  else if (sensorLeft==follow && sensorRight!=follow && sensor3!=follow && sensor4!=follow)
  {  
    //digitalWrite (leftLed,HIGH);
    while(sensor2!=follow && sensor3!=follow && sensor4!=follow && sensor5!=follow)
    { 
      turnLeft();
      //delay(300);
      senser();
    }
  }
  
  
  //Turn Right Condition
  else if (sensorRight==follow && sensorLeft!=follow && sensor3!=follow && sensor3!=follow)
  { 
    digitalWrite (rightLed,HIGH);
     while (sensor2!=follow && sensor3!=follow && sensor4!=follow && sensor5!=follow)
    {
      turnRight();
      senser();
    } 
    
  }
  
  
  
  
  
  
  // Keeping robot in track code
  else if(sensor1== follow || sensor2== follow || sensor3== follow || sensor4== follow || sensor5== follow || sensor6==follow)
  {
    SpeedMax=150;
    Kp = 10; //50
    Kd = 50; //100
    Ki = 0;
  }
  
  
  
  else 
  {
    Kp = 100;
    Kd = 0;
    Ki = 0;
  }
  
   
   }
   
   else  if(sensorLeft==follow && sensorRight==follow) // left and right && left right and straight condition
   {
     if (memory[counter]=='S')
     {
       while(sensorLeft==follow)
       {
         goStraight();
         senser();        
       }
       counter--;
     }
     else if (memory[counter]=='R')
     {
       while(sensorLeft==follow)
       {
         turnLeft();
         senser();
       }
       counter--;
     }
     else if (memory[counter]=='L')
     {
       while(sensorRight==follow)
       {
         turnRight();
         senser();
       }
       counter--;
     }
   }
   else if (sensorLeft==follow  && (sensor3 == follow && sensor4==follow)) // left and straight condtion
   {
     if(memory[counter]=='S')
    { 
     while(sensorLeft==follow)
       {
         goStraight();
         senser();        
       }
       counter--;
     }
     else if (memory[counter]=='R')
     {
       while(sensorLeft==follow)
       {
         turnLeft();
         senser();
       }
       counter--;
     }
     else if (memory[counter]=='L')
     {
       while(sensorRight==follow)
       {
         turnRight();
         senser();
       }
       counter--;
     }
    
   }
   
   else if(sensorLeft !=follow && sensorRight==follow && sensor3 == follow && sensor4 == follow) // straight and right condition 
   { 
     if(memory[counter]=='S')
     {
      while(sensorLeft==follow)
       {
         goStraight();
         senser();        
       }
       counter--;
     }
     else if (memory[counter]=='R')
     {
       while(sensorLeft==follow)
       {
         turnLeft();
         senser();
       }
       counter--;
     }
     else if (memory[counter]=='L')
     {
       while(sensorRight==follow)
       {
         turnRight();
         senser();
       }
       counter--;
     }
   }
   
  // Turn left condition without intersection. 
  else if (sensorLeft==follow && sensorRight!=follow && sensor3!=follow && sensor4!=follow)
  {  
    //digitalWrite (leftLed,HIGH);
    while(sensor2!=follow && sensor3!=follow && sensor4!=follow && sensor5!=follow)
    { 
      turnLeft();
      //delay(300);
      senser();
    }
  }
  
  
  //Turn Right Condition
  else if (sensorRight==follow && sensorLeft!=follow && sensor3!=follow && sensor3!=follow)
  { 
    digitalWrite (rightLed,HIGH);
     while (sensor2!=follow && sensor3!=follow && sensor4!=follow && sensor5!=follow)
    {
      turnRight();
      senser();
    } 
    
  }
  
  
  
  
  
  
  // Keeping robot in track code
  else if(sensor1== follow || sensor2== follow || sensor3== follow || sensor4== follow || sensor5== follow || sensor6==follow)
  {
    SpeedMax=150;
    Kp = 10; //50
    Kd = 50; //100
    Ki = 0;
  }
  
  
  
  
  else 
  {
    Kp = 100;
    Kd = 0;
    Ki = 0;
  }
  
  }
  
  
  
  
  error =  (-4*sensor0)  +(-3*sensor1)  + (-2*sensor2) + (-1*sensor3) + (1*sensor4) + (2*sensor5) + (-3*sensor6)  +(4*sensor7);
  derror = error - preverror;
  ierror += error; 
  U = Kp * error + Kd * derror + Ki * ierror;  
  preverror = error;
  
  

  
  
  if(U<-SpeedMax) U=-SpeedMax;
  if(U>SpeedMax) U=SpeedMax;
  
 
  
  
  if(U>0)
  {
    motor1.run(FORWARD);
    motor1.setSpeed(SpeedMax);
  
    motor2.run(FORWARD);
    motor2.setSpeed(SpeedMax-U);
  
  }
  else if(U<0)
  {
    motor1.run(FORWARD);
    motor1.setSpeed(SpeedMax+U);
  
    motor2.run(FORWARD);
    motor2.setSpeed(SpeedMax);
 
  }
  else if (error==0) 
  {
    motor1.run(FORWARD);
    motor1.setSpeed(SpeedMax);
  
    motor2.run(FORWARD);
    motor2.setSpeed(SpeedMax);
 
  }
  else if (U==0) 
  {
    motor1.run(FORWARD);
    motor1.setSpeed(SpeedMax);
  
    motor2.run(FORWARD);
    motor2.setSpeed(SpeedMax);
 
  }   
  
  
}
void senser()
{
   sensor0 = analogRead(8);
   if ( sensor0<crit ) sensor0=0;
   else sensor0=1;
   Serial.print(sensor0);
   Serial.print(' ');
  
    
   sensor1 = analogRead(9);
   if ( sensor1<crit ) sensor1=0;
   else sensor1=1;
   Serial.print(sensor1);
   Serial.print(' ');
   
   
   
   sensor2 = analogRead(10);
   if ( sensor2<crit ) sensor2=0;
   else sensor2=1;
   Serial.print(sensor2);
   Serial.print(' ');
   
   
   sensor3 = analogRead(11);
   if ( sensor3<crit ) sensor3=0;
   else sensor3=1;
   Serial.print(sensor3);
   Serial.print(' ');
   
   
   sensor4 = analogRead(12);
   if ( sensor4<crit ) sensor4=0;
   else sensor4=1;
   Serial.print(sensor4);
   Serial.print(' ');
   
   
   sensor5 = analogRead(13);
   if ( sensor5<crit ) sensor5=0;
   else sensor5=1;
   Serial.print(sensor5);
   Serial.print(' ');
  
   sensor6 = analogRead(14);
   if ( sensor6<crit ) sensor6=0;
   else sensor6=1;
   Serial.print(sensor6);
   Serial.print(' ');
   
   sensor7 = analogRead(15);
   Serial.print(sensor7);
   Serial.println(' ');
   if ( sensor7<crit ) sensor7=0;
   else sensor7=1;
   
   sensorFL = digitalRead(53);
   Serial.print(sensorFL);
   Serial.print(' ');
   
   
   sensorFR = digitalRead(52);
   Serial.print(sensorFR);
   Serial.print(' ');
   
   sensorLeft = digitalRead(51);
   Serial.print(sensorLeft);
   Serial.print(' ');
   
   sensorRight = digitalRead(48);
   Serial.print(sensorRight);
   Serial.println(' ');
   
   
   //delay (1000) ; 
}

void pathShort()
{
  for(int i=0; i<=counter;i++)
  {
    if (memory[i]=='U')
    {  
      if (memory[i-1]=='U')
      {
        for(int j=i;j<counter-1;j++)
        {
          memory[j]=memory[j+1];
        }
        counter=counter-1;
      }
       if (memory[i-1]=='L' && memory[i+1]=='S')
      {
        memory[i-1]='R';
        i= i-2;
        for(int j=i;j<counter-2;j++)
        {
          memory[j]=memory[j+2];
        }
        counter=counter-2;
      }
      
       if (memory[i-1]=='L' && memory[i+1]=='L')
      {
      memory[i-1]='S';
      i= i-2;
      for(int j=i;j<counter-2;j++)
      {
      memory[j]=memory[j+2];
      }
      counter=counter-2;
      }
      
       if (memory[i-1]=='L' && memory[i+1]=='R')
      {
      memory[i-1]='U';
      i= i-2;
      for(int j=i;j<counter-2;j++)
      {
      memory[j]=memory[j+2];
      }
      counter=counter-2;
      }
       if (memory[i-1]=='R' && memory[i+1]=='L')
      {
      memory[i-1]='U';
      i= i-2;
      for(int j=i;j<counter-2;j++)
      {
      memory[j]=memory[j+2];
      }
      counter=counter-2;
      }
       if (memory[i-1]=='R' && memory[i+1]=='R')
      {
      memory[i-1]='S';
      i= i-2;
      for(int j=i;j<counter-2;j++)
      {
      memory[j]=memory[j+2];
      }
      counter=counter-2;
      }
       if (memory[i-1]=='R' && memory[i+1]=='S')
      {
      memory[i-1]='L';
      i= i-2;
      for(int j=i;j<counter-2;j++)
      {
      memory[j]=memory[j+2];
      }
      counter=counter-2;
      }
       if (memory[i-1]=='S' && memory[i+1]=='L')
      {
      memory[i-1]='R';
      i= i-2;
      for(int j=i;j<counter-2;j++)
      {
      memory[j]=memory[j+2];
      }
      counter=counter-2;
      }
       if (memory[i-1]=='S' && memory[i+1]=='R')
      {
      memory[i-1]='L';
      i= i-2;
      for(int j=i;j<counter-2;j++)
      {
      memory[j]=memory[j+2];
      }
      counter=counter-2;
      }
       if (memory[i-1]=='S' && memory[i+1]=='S')
      {
      memory[i-1]='U';
      i= i-2;
      for(int j=i;j<counter-2;j++)
      {
      memory[j]=memory[j+2];
      }
      counter=counter-2;
      }
      
    }
  }
}

void printPath()
{
  for (int i=0;i<counter;i++)
  {
  Serial.print(memory[i]);
  Serial.print(" ");
  }
  Serial.println("");
}


void Stop()
{  
  
    motor1.run(FORWARD);
    motor1.setSpeed(0);
  
    motor2.run(FORWARD);
    motor2.setSpeed(0);

}

void turnRight()
{ 
    motor1.run(BACKWARD);
    motor1.setSpeed(100);
  
    motor2.run(FORWARD);
    motor2.setSpeed(100);
}


void turnLeft()
{ 
    motor1.run(FORWARD);
    motor1.setSpeed(100);
  
    motor2.run(BACKWARD);
    motor2.setSpeed(100);
}

void goStraight()
{ 
    motor1.run(FORWARD);
    motor1.setSpeed(SpeedMax);
  
    motor2.run(FORWARD);
    motor2.setSpeed(SpeedMax);
}

void turnU()
{ 
    motor1.run(BACKWARD);
    motor1.setSpeed(100);
  
    motor2.run(FORWARD);
    motor2.setSpeed(100);
}

