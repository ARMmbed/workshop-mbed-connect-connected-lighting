#include "mbed.h"
#include "MMA7660FC.h"

MMA7660FC acc(I2C_SDA, I2C_SCL, 0x98);
static Thread accelThread;

class Accelerometer {
public:
  Accelerometer(int aThreshold = 10, bool aDebug = false)
      : threshold(aThreshold), debug(aDebug) {
  }

  void start() {
    accelThread.start(this, &Accelerometer::main);
  }

  void change(void (*cb)(void)) {
    callback = cb;
  }

private:
  void main() {

    wait_ms(3000); // let the sensor warm up...

    int v = acc.check();
    if (v != 0) {
      printf("Loading accelerometer failed... %d\r\n", v);
    }


    acc.init();

    int lastX = 0, lastY = 0, lastZ = 0;
    bool firstEvent = true;

    while (1) {
      int x = acc.read_x(), y = acc.read_y(), z = acc.read_z();

      int delta = abs(lastX + lastY + lastZ) - abs(x + y + z);
      
      if (delta > threshold) {
        printf("delta is %d\r\n", delta);
        printf("x=%d y=%d z=%d lx=%d ly=%d lz=%d\r\n", x, y, z, lastX, lastY, lastZ);
      }

      lastX = x;
      lastY = y;
      lastZ = z;

      if (delta > threshold) {
        if (firstEvent) {
          firstEvent = false;
          wait_ms(5);
          continue;
        }

        if (debug) {
          // printf("delta is %d\r\n", delta);
          // printf("x=%d y=%d z=%d\r\n", lastX, lastY, lastZ);
        }

        if (callback) callback();

        wait_ms(1000);

        lastX = acc.read_x();
        lastY = acc.read_y();
        lastZ = acc.read_z();
      }
      else {
        wait_ms(20);
      }
    }
  }

  bool debug;
  int threshold;
  void (*callback)(void);
};
