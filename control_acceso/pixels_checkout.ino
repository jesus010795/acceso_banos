void pixels_checkout() {

  pixels.clear();  // Set all pixel colors to 'off'
  for (int i = 0; i < NUMPIXELS; i++) {
    pixels.setPixelColor(i, pixels.Color(0, 0, 150));
    pixels.show();    // Send the updated pixel colors to the hardware.
    delay(100);  // Pause before next pass through loop
  }
}