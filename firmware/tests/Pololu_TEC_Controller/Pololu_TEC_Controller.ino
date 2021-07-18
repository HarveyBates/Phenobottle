// Just some base code for controlling my TEC Module


char INA1 = 23;
char INA2 = 22;
char PWMA = 21;


void setup() {
  Serial.begin(9600);
  pinMode(INA1, OUTPUT);
  pinMode(INA2, OUTPUT);
  pinMode(PWMA, OUTPUT);
  pinMode(13, OUTPUT);
  digitalWrite(PWMA, HIGH); // Turn motor on
  digitalWrite(INA1, LOW);
  analogWrite(INA2, 50);
}

void loop() {
  digitalWrite(13, HIGH);
  delay(1000);
  digitalWrite(13, LOW);
  delay(2000);
}
