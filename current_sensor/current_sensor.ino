void setup(){
  pinMode(A0, INPUT);
  Serial.begin(9600);
}
void loop(){
  int analogvaluetemp;
  float analogvalue;
  float current;
  analogvaluetemp=analogRead(A0);
  delay(10);
  analogvaluetemp=analogRead(A0);
  analogvaluetemp=analogRead(A0);
  analogvalue=analogvaluetemp*(4.89/1024.0);
  current=analogvalue;
  
  Serial.println(current);
}
