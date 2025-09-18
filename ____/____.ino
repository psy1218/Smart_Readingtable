void setup(){
  pinMode(13, OUTPUT);
}

void loop(){
  digitalWrite(13, HIGH);
  delay(100);
  digitalWrite(13, LOW);
  delay(100);
  digitalWrite(13, HIGH);
  delay(200);
  digitalWrite(13, LOW);
  delay(100);
   digitalWrite(13, HIGH);
  delay(300);
  digitalWrite(13, LOW);
  delay(100);
  digitalWrite(13, HIGH);
  delay(400);
  digitalWrite(13, LOW);
  delay(1000);
}
