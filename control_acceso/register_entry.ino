void register_entry() {

  DateTime time = rtc.now();

  myFile = SD.open("registro.txt", FILE_WRITE);

  if (myFile) {
    Serial.println("Escribiendo fecha y hora de entrada en prueba: ");

    myFile.print(daysOfTheWeek[time.dayOfTheWeek()]);
    
    myFile.print("/");
    myFile.print(first_name);
    myFile.print(last_name);
    myFile.print("/");
    myFile.print(time.timestamp(DateTime::TIMESTAMP_DATE));
    myFile.print("/");
    myFile.print(time.timestamp(DateTime::TIMESTAMP_TIME));
    myFile.print("/");

    myFile.close();
    delay(100);
    Serial.println("done.");
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening testRegistro.txt");
  }
}