#include <Servo.h> 
Servo myservo;  

#define BRAKE 0
#define CW    1
#define CCW   2
#define CS_THRESHOLD 15   // Definition of safety current (Check: "1.3 Monster Shield Example").

//MOTOR 1
#define MOTOR_A1_PIN 7
#define MOTOR_B1_PIN 8

#define PWM_MOTOR_1 5

#define CURRENT_SEN_1 A2

#define EN_PIN_1 A0

#define MOTOR_1 0

short usSpeed = 40;  //default motor speed
unsigned short usMotor_Status = BRAKE;

//Servo

#define ServoPin 9
 
void setup()                         
{
  pinMode(MOTOR_A1_PIN, OUTPUT);
  pinMode(MOTOR_B1_PIN, OUTPUT);

  pinMode(PWM_MOTOR_1, OUTPUT);
  
  pinMode(CURRENT_SEN_1, OUTPUT);
 
  pinMode(EN_PIN_1, OUTPUT);

  myservo.write(1500);
   
  myservo.attach(ServoPin);

  Serial.begin(9600);              // Initiates the serial to do the monitoring 
  Serial.println("Begin motor control");
  Serial.println(); //Print function list for user selection
  Serial.println("Nhap su lua chon:");
  Serial.println("0. Di thang");
  Serial.println("1. Re trai");
  Serial.println("2. Re phai");
  Serial.println("3. Dung");
  Serial.println("4. Tien");
  Serial.println("5. Lui");
  Serial.println("+. Tang toc do (10)");
  Serial.println("-. Giam toc do 10");
  Serial.println();
  myservo.write(90);
}

void loop() 
{
  char user_input;   
  while(Serial.available())
  {
    user_input = Serial.read(); //Read user input and trigger appropriate function
    digitalWrite(EN_PIN_1, HIGH);

    if (user_input =='0')
    {
      GoHead();
    }
    else if (user_input =='1')
    {
       TurnLeft();
    }
    else if (user_input =='2')
    {
       TurnRight();
    }
    else if (user_input =='3')
    {
       Stop();
    }
    else if(user_input =='4')
    {
      Forward();
    }
    else if(user_input =='5')
    {
      Reverse();
    }
    else if(user_input =='+')
    {
      IncreaseSpeed();
    }
    else if(user_input =='-')
    {
      DecreaseSpeed();
    }
    else
    {
      Serial.println("Invalid option entered.");
    }
  }
}

void GoHead()
{
  Serial.println("Di thang");
  myservo.write(90);
}
void TurnLeft()
{
  Serial.println("Re trai");
  myservo.write(30);
}

void TurnRight()
{
  Serial.println("Re phai");
  myservo.write(150);
}

void Stop()
{
  Serial.println("Stop");
  usMotor_Status = BRAKE;
  motorGo(MOTOR_1, usMotor_Status, 0);
}

void Forward()
{
  Serial.println("Forward");
  usMotor_Status = CW;
  motorGo(MOTOR_1, usMotor_Status, usSpeed);
}

void Reverse()
{
  Serial.println("Reverse");
  usMotor_Status = CCW;
  motorGo(MOTOR_1, usMotor_Status, usSpeed);
}

void IncreaseSpeed()
{
  usSpeed = usSpeed + 10;
  if(usSpeed > 255)
  {
    usSpeed = 255;  
  }
  
  Serial.print("Speed +: ");
  Serial.println(usSpeed);

  motorGo(MOTOR_1, usMotor_Status, usSpeed);
}

void DecreaseSpeed()
{
  usSpeed = usSpeed - 10;
  if(usSpeed < 0)
  {
    usSpeed = 0;  
  }
  
  Serial.print("Speed -: ");
  Serial.println(usSpeed);

  motorGo(MOTOR_1, usMotor_Status, usSpeed);
}

void motorGo(uint8_t motor, uint8_t direct, uint8_t pwm)         
//Function that controls the variables: motor(0 ou 1), direction (cw ou ccw) e pwm (entra 0 e 255);
{
  if(motor == MOTOR_1)
  {
    if(direct == CW)
    {
      digitalWrite(MOTOR_A1_PIN, LOW); 
      digitalWrite(MOTOR_B1_PIN, HIGH);
    }
    else if(direct == CCW)
    {
      digitalWrite(MOTOR_A1_PIN, HIGH);
      digitalWrite(MOTOR_B1_PIN, LOW);      
    }
    else
    {
      digitalWrite(MOTOR_A1_PIN, LOW);
      digitalWrite(MOTOR_B1_PIN, LOW);            
    }
    
    analogWrite(PWM_MOTOR_1, pwm); 
  }
}


