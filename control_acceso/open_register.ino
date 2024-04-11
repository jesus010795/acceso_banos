void open_register(){
     delay(500);
    if (id_employee != "") {
      myFile = SD.open("registro.txt");
      if (myFile) {
        Serial.println("testRegistro.txt:");

        // read from the file until there's nothing else in it:
        while (myFile.available()) {
          Serial.write(myFile.read());
        }
        // close the file:
        myFile.close();
      } else {
        // if the file didn't open, print an error:
        Serial.println("error opening testRegistro.txt");
      }
    }
    delay(500);
}