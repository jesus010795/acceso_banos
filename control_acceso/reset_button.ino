// void reset_button() {
//   DateTime time = rtc.now();
//   myFile = SD.open("registro.txt", FILE_WRITE);

//   if (myFile) {
//     // Serial.println("Boton reset");
//     myFile.print(daysOfTheWeek[time.dayOfTheWeek()]);
//     myFile.print("/");
//     myFile.print("BOTON RESET");
//     myFile.print("-----------");
//     myFile.print("/");
//     myFile.print(time.timestamp(DateTime::TIMESTAMP_DATE));
//     myFile.print("/");
//     myFile.print(time.timestamp(DateTime::TIMESTAMP_TIME));
//     myFile.print("/");
//     myFile.println("--------");

//     myFile.close();
//     delay(100);
//     // Serial.println("done.");
//   } else {
//     // if the file didn't open, print an error:
//     Serial.println("error opening testRegistro.txt");
//   }
// }