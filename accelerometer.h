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

      int delta = abs(lastX - x) + abs(lastY - y) + abs(lastZ - z);

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
          printf("delta is %d\r\n", delta);
        }

        if (callback) callback();

        wait_ms(1000);

        lastX = acc.read_x();
        lastY = acc.read_y();
        lastZ = acc.read_z();
      }
      else {
        wait_ms(5);
      }
    }
  }

  bool debug;
  int threshold;
  void (*callback)(void);
};
