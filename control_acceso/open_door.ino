void open_door(){
  digitalWrite(lock, LOW); 
  Serial.println("Puerta abierta");
  delay(5000);           
  digitalWrite(lock, HIGH);  
  delay(100);    
}