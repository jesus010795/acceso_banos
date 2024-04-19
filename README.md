# Sistema control de acceso

El sistema implementado es un controlador de acceso que utiliza tecnología RFID para autorizar la entrada de usuarios a un espacio específico, como un baño. Está diseñado para ser controlado por un Arduino Mega y emplea componentes como un lector RFID, una matriz de LEDs NeoPixel, una tarjeta SD para almacenamiento, un reloj de tiempo real (RTC) para marcar la hora de entrada y salida de los usuarios, y un buzzer para notificaciones auditivas.

## Componentes del Sistema

1. **Arduino Mega:** Actúa como el controlador principal del sistema, coordinando todas las operaciones y procesando la información de los demás componentes.
2. **Lector de Tarjetas RFID:** Permite la identificación de usuarios autorizados mediante tarjetas RFID.
3. **Matriz de LEDs NeoPixel:** Proporciona retroalimentación visual sobre el estado del sistema, como la ocupación del baño.
4. **Tarjeta SD:** Utilizada para almacenar registros de entrada y salida de usuarios.
5. **Reloj de Tiempo Real (RTC):** Suministra la hora actual para registrar con precisión los eventos de acceso.
6. **Buzzer:** Proporciona notificaciones auditivas para indicar eventos como acceso concedido o error en la lectura de la tarjeta RFID.

## Librerías utilizadas:

- **SPI.h**: Esta librería permite la comunicación SPI entre dispositivos.
- **MFRC522.h**: Librería para el manejo del lector RFID.
- **RTClib.h**: Librería para el manejo del RTC.
- **SD.h**: Librería para el manejo de la tarjeta SD.
  **Adafruit_NeoPixel.h**: Librería para controlar matrices de LED NeoPixel.

### Variables y constantes definidas

- **PINPIXELS**: Pin utilizado para controlar los LEDs NeoPixel.
- **NUMPIXELS**: Número de LEDs NeoPixel en la matriz.
- **PIN_SD**: Pin utilizado para la comunicación con la tarjeta SD.
- **RST_PIN**: Pin de reset del lector RFID.
- **SS_PIN**: Pin de selección de esclavo para el lector RFID.
- **BUZZER**: Pin utilizado para el zumbador.
- **BTN**: Pin utilizado para el botón.

### Objetos y variables globales

- **rtc**: Objeto del reloj de tiempo real.
- **daysOfTheWeek**: Matriz de días de la semana en varios idiomas.
- **myFile**: Objeto para el manejo de archivos en la tarjeta SD.
- **mfrc522**: Objeto del lector RFID.
- **key**: Clave para la comunicación con la tarjeta RFID.
- **lock**: Pin utilizado para controlar el cerrojo.

### Funciones principales

- **setup()**: Configuración inicial del sistema, inicialización de periféricos y comprobación de su correcto funcionamiento.
- **loop()**: Bucle principal del programa que realiza las operaciones de lectura de tarjeta RFID, control de acceso y registro de entradas y salidas.

### Funciones auxiliares

- **buzzer_error()**, **buzzer_ok()**, **buzzer_alert()**: Funciones para controlar el zumbador en diferentes situaciones.
- **get_hex_id()**, **get_first_name()**, **get_last_name()**: Funciones para obtener información de la tarjeta RFID.
- **register_entry()**, **open_door()**, **check_out()**: Funciones para registrar la entrada, abrir la puerta y registrar la salida, respectivamente.

## Funcionamiento

1. **Inicialización del Sistema:**
    - Se inicializan todos los componentes del sistema, incluyendo el Arduino Mega, el RTC, el lector RFID, la matriz de LEDs NeoPixel, la tarjeta SD y el buzzer.
    - Se realizan comprobaciones para garantizar que todos los componentes estén operativos.

### Variables

2.  **Bucle Principal:**

    - El Arduino Mega entra en un bucle principal donde espera la detección de tarjetas RFID.
    - Se utiliza una matriz de LEDs NeoPixel para indicar el estado del sistema, como la disponibilidad del baño.

3. **Autorización de Acceso:**

    - Cuando se detecta una tarjeta RFID, el sistema verifica su identificación y consulta la base de datos para determinar si el usuario está autorizado.
    - Si la tarjeta es válida, se registra la entrada del usuario en la tarjeta SD junto con la marca de tiempo.
    - Se activan indicadores visuales y sonoros, incluyendo el buzzer, para confirmar el acceso concedido.

4. **Control de Acceso Ocupado**

    - Mientras el baño esté ocupado, el sistema bloquea el lector RFID para evitar intentos de acceso no autorizados.
    - Se espera la detección del botón de salida para permitir la salida del usuario.

5. **Registro en la Tarjeta SD**

    - Cada entrada y salida de usuarios se registra en un archivo de texto en la tarjeta SD.
    - El registro incluye la identificación del usuario, la fecha y hora de entrada y salida.

## Funcionalidad del codigo principal

1. `if (mfrc522.PICC_IsNewCardPresent() & mfrc522.PICC_ReadCardSerial()) {`: Esta línea comprueba si hay una nueva tarjeta presente y si se puede leer su contenido.

2. `id_employee = get_hex_id();`: Se llama a la función `get_hex_id()` para obtener la identificación del empleado en formato hexadecimal desde la tarjeta RFID y se asigna a la variable `id_employee`.

3. `first_name = get_first_name();`: Se llama a la función `get_first_name()` para obtener el nombre del empleado desde la tarjeta RFID y se asigna a la variable `first_name`.

4. `last_name = get_last_name();`: Se llama a la función `get_last_name()` para obtener el apellido del empleado desde la tarjeta RFID y se asigna a la variable `last_name`.

5. `if (id_employee != "") {`: Se verifica si se ha obtenido con éxito la identificación del empleado.

6. `pixels.clear();`: Se limpian todos los píxeles de la matriz de LEDs NeoPixel para apagar cualquier LED que esté encendido.

7. Se establecen los primeros cuatro píxeles de la matriz de LEDs NeoPixel en color rojo, indicando que el baño está ocupado.

8. Se ejecutan varias acciones relacionadas con el acceso al baño:

    - `buzzer_ok()`: Se emite un tono de confirmación a través del buzzer para indicar que se ha autorizado el acceso.
    - `open_door()`: Se abre la puerta del baño.
    - `register_entry()`: Se registra la entrada del empleado en el sistema.
9. Se establece el LED verde en LOW y el LED rojo en HIGH para indicar visualmente que el baño está ocupado.

10. `while (occupied_bathroom) {`: Se inicia un bucle while para manejar el estado del baño mientras esté ocupado.

11. `if (digitalRead(BTN) == HIGH) {`: Se comprueba si se ha presionado el botón de salida.

12. Se ejecutan una serie de acciones cuando se presiona el botón de salida:

- Se detiene la lectura de la tarjeta RFID.
- Se establece una variable booleana `compare` en true para iniciar un bucle de comparación entre la tarjeta actual y la tarjeta de salida.
- Se emite un tono de confirmación a través del buzzer.
- Se abre la puerta del baño.

13. Dentro del bucle de comparación:
  Se comprueba continuamente si se presenta una nueva tarjeta RFID.
- Se obtiene la identificación de la nueva tarjeta y se compara con la identificación del empleado.
- Si las identificaciones coinciden, se ejecutan acciones para registrar la salida del empleado y se detiene el bucle de comparación.

14. Una vez que el empleado ha salido del baño, se establece la variable `occupied_bathroom` en false para salir del bucle principal.

15. `mfrc522.PICC_HaltA();` y `mfrc522.PCD_StopCrypto1();`: Se detiene la comunicación con la tarjeta RFID.

16. Si no se detecta una nueva tarjeta presente, se detiene la comunicación con la tarjeta RFID y se sale del bucle principal.

*Hay partes del código que están comentadas y parecen son fragmentos de funcionalidades que estaban en desarrollo y fueron desactivadas temporalmente.*
