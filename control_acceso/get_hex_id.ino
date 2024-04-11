String get_hex_id() {

  to_string(mfrc522.uid.uidByte, mfrc522.uid.size, id_employee);

  return id_employee;
}

