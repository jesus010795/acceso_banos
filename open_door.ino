void open_door(){
  digitalWrite(lock, LOW);   // activar el rele'
  Serial.println("Puerta abierta");
  delay(5000);               // esperar un segundo
  digitalWrite(lock, HIGH);    // apagar el rele'
  delay(100);    
}