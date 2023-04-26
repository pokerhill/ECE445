
//.27 ->Green
//A4 --> Yellow
//A5 --> Red
#define GREEN (27)
#define GREEN2 (26)
void setup() {
  // put your setup code here, to run once:
  pinMode(GREEN, OUTPUT);
  pinMode(A4, OUTPUT);
  pinMode(A5, OUTPUT);
  digitalWrite(GREEN,LOW);
  digitalWrite(GREEN2,LOW);
  digitalWrite(A4, LOW);
  digitalWrite(A5, LOW);
}

void loop() {
  // put your main code here, to run repeatedly:

  digitalWrite(GREEN, HIGH);
  digitalWrite(GREEN2, HIGH);
  delay(1000);
  digitalWrite(GREEN, LOW);
  digitalWrite(GREEN2, LOW);
  delay(1000);
  digitalWrite(A4, HIGH);
  delay(1000);
  digitalWrite(A4, LOW);
  delay(1000);
  digitalWrite(A5, HIGH);
  delay(1000);
  digitalWrite(A5, LOW);
  delay(1000);
  

}
