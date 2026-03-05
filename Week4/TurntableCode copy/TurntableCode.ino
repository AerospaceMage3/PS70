// Motor 1
int A1A = D7;
int A1B = D8;
int potPin = D0;

// const int freq = 5000;
// const int resolution = 8;


void setup() {
  pinMode(A1A, OUTPUT);  
  pinMode(A1B, OUTPUT); 
  Serial.begin(9600);

  // ledcAttach(A1A, freq, resolution);
  // ledcWrite(A1A, 0);   // start with the motors off 
  analogWrite(A1A, 0);
  digitalWrite(A1B, LOW);
}

void loop() {
  int pot_value = analogRead(potPin);   //pot wiper is on A2
  Serial.println(pot_value);
  int motor_speed = map(pot_value, 0, 4096, 255, 0); // map motor_level to pot_value so that zero corresponds to lowest speed
  analogWrite(A1A, motor_speed);
  delay(1);
}