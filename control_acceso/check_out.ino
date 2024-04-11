void check_out() {
  DateTime time = rtc.now();
  myFile = SD.open("registro.txt", FILE_WRITE);
  

  Serial.println("Escribiendo hora de salida en prueba: ");

  if (myFile) {
    myFile.println(time.timestamp(DateTime::TIMESTAMP_TIME));


    myFile.close();
    delay(100);
    Serial.println("done.");
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening testRegistro.txt");
  }
}