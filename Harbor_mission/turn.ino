void _TR() {
  ReadLine();
  Motors(100, -100);
  while (sensor_values[3] < 500) {
    ReadLine();
  }
  Motors(100, -100);
  while (sensor_values[3] > 500) {
    ReadLine();
  }
  Motors(100, -100);
  while (sensor_values[3] < 500) {
    ReadLine();
  }
  AO();
}
void _TL() {
  ReadLine();
  Motors(-100, 100);
  while (sensor_values[4] < 500) {
    ReadLine();
  }
  Motors(-100, 100);
  while (sensor_values[4] > 500) {
    ReadLine();
  }
  Motors(-100, 100);
  while (sensor_values[4] < 500) {
    ReadLine();
  }
  AO();
}