int sensor_back(int8_t i) {
  int sensor_value = adc.read(i);                                                                        // อ่านค่า analog จากเซนเซอร์ที่ตำแหน่ง i
  int map_sensor_value = map(sensor_value, max_back_sensor_value[i], min_back_sensor_value[i], 0, 100);  // แปลงค่าให้อยู่ในช่วง 0-100
  return map_sensor_value >= 50 ? 1 : 0;                                                                 // ถ้าค่ามากกว่า 50 ถือว่าเซนเซอร์ตรวจพบเส้น(เจอสีดำ)
}

int Position_back() {
  int sum = 0;
  int avg = 0;
  int value;

  for (int8_t i = 0; i < numsensor_back; i++) {
    value = sensor_back(i);     // Read sensor value (0 or 1)
    avg += value * (i * 100);    // Apply weight (0, 100, 200, 300, 400)
    sum += value;                // Sum the active sensors
  }

  // Check if any sensor is on the line
  if (sum > 0) {
    last_position = avg / sum;   // Calculate weighted average
  } else {
    // Avoid division by zero, use last known position
    if (last_position >= setpoint_back) {
      last_position = 700;  // Assume far right if last position was on the right
    } else {
      last_position = 0;    // Assume far left if last position was on the left
    }
  }

  return last_position;
}

void backward(int BaseSpeed, float Kp, float Ki, float Kd) {
  int8_t integral = 0;
  int8_t output = 0;
  int last_error = 0;
  while (sensor_back(0) == 0 || sensor_back(2) == 0 || sensor_back(4) == 0 || sensor_back(6) == 0) {
    int error = Position_back() - setpoint_back;
    integral += error;
    integral = constrain(integral, -100, 100);
    output = (error * Kp) + (integral * Ki) + ((error - last_error) * Kd);
    last_error = error;
    leftmotor = -BaseSpeed + output;
    rightmotor = -BaseSpeed - output;
    leftmotor = constrain(leftmotor, -70, 70);
    rightmotor = constrain(rightmotor, -70, 70);
    Motor(leftmotor, rightmotor);
  }
  Motor(-40, -40);
  delay(60);
  AO();
  delay(200);
}