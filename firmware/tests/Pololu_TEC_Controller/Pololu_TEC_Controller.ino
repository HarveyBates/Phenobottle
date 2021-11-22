// Just some base code for controlling my TEC Module


uint8_t INA1 = 23;
uint8_t INA2 = 22;

uint8_t PWMA = 21;


void setup() {
  Serial.begin(9600);
  analogWriteResolution(10);
  pinMode(INA1, OUTPUT);
  pinMode(INA2, OUTPUT);
  pinMode(PWMA, OUTPUT);
  
  pinMode(13, OUTPUT);
  
  digitalWrite(PWMA, HIGH); // Turn motor on
//  digitalWrite(INA1, LOW);
  digitalWrite(INA2, LOW);
  
//  analogWrite(INA2, 1023); // 1023
  analogWrite(INA1, 1023);
}

void loop() {
  digitalWrite(13, HIGH);
  delay(1000);
  digitalWrite(13, LOW);
  delay(2000);
}
