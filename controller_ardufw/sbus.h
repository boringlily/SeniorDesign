#ifndef SBUS_H
#define SBUS_H

#include "includes.h"

// SBUS TIMER //
hw_timer_t * timer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;
 
void IRAM_ATTR onTimer();
// END SBUS TIMER //

struct SbusData {
  bool lost_frame;
  bool failsafe;
  bool ch17, ch18;
  static constexpr int8_t NUM_CH = 16;
  int16_t ch[NUM_CH];
};

static constexpr uint8_t HEADER_ = 0x0F;
static constexpr uint8_t FOOTER_ = 0x00;
static constexpr uint8_t FOOTER2_ = 0x04;
static constexpr uint8_t CH17_MASK_ = 0x01;
static constexpr uint8_t CH18_MASK_ = 0x02;
static constexpr uint8_t LOST_FRAME_MASK_ = 0x04;
static constexpr uint8_t FAILSAFE_MASK_ = 0x08;

uint8_t buf[BUF_SIZE];
SbusData data_;

void writeSbus(void * parameter)
{
  Serial2.write(buf, BUF_SIZE);
  vTaskDelete(NULL);
}

void IRAM_ATTR onTimer() {
  portENTER_CRITICAL_ISR(&timerMux); //Maybe not necessary...
  xTaskCreate(
      writeSbus,    // Function that should be called
      "Write SBUS",  // Name of the task (for debugging)
      1000,            // Stack size (bytes)
      NULL,            // Parameter to pass
      1,               // Task priority
      NULL             // Task handle
  );
  portEXIT_CRITICAL_ISR(&timerMux);
}


void sbus_to_buf(SbusData data_, uint8_t* buf_)
{
  buf_[0] = HEADER_;
  buf_[1] = static_cast<uint8_t>((data_.ch[0] & 0x07FF));
  buf_[2] = static_cast<uint8_t>((data_.ch[0] & 0x07FF) >> 8 | (data_.ch[1] & 0x07FF) << 3);
  buf_[3] = static_cast<uint8_t>((data_.ch[1] & 0x07FF) >> 5 | (data_.ch[2] & 0x07FF) << 6);
  buf_[4] = static_cast<uint8_t>((data_.ch[2] & 0x07FF) >> 2);
  buf_[5] = static_cast<uint8_t>((data_.ch[2] & 0x07FF) >> 10 | (data_.ch[3] & 0x07FF) << 1);
  buf_[6] = static_cast<uint8_t>((data_.ch[3] & 0x07FF) >> 7 | (data_.ch[4] & 0x07FF) << 4);
  buf_[7] = static_cast<uint8_t>((data_.ch[4] & 0x07FF) >> 4 | (data_.ch[5] & 0x07FF) << 7);
  buf_[8] = static_cast<uint8_t>((data_.ch[5] & 0x07FF) >> 1);
  buf_[9] = static_cast<uint8_t>((data_.ch[5] & 0x07FF) >> 9  | (data_.ch[6] & 0x07FF) << 2);
  buf_[10] = static_cast<uint8_t>((data_.ch[6] & 0x07FF) >> 6  | (data_.ch[7] & 0x07FF) << 5);
  buf_[11] = static_cast<uint8_t>((data_.ch[7] & 0x07FF) >> 3);
  buf_[12] = static_cast<uint8_t>((data_.ch[8] & 0x07FF));
  buf_[13] = static_cast<uint8_t>((data_.ch[8] & 0x07FF) >> 8 | (data_.ch[9]  & 0x07FF) << 3);
  buf_[14] = static_cast<uint8_t>((data_.ch[9] & 0x07FF) >> 5 | (data_.ch[10] & 0x07FF) << 6);
  buf_[15] = static_cast<uint8_t>((data_.ch[10] & 0x07FF) >> 2);
  buf_[16] = static_cast<uint8_t>((data_.ch[10] & 0x07FF) >> 10 | (data_.ch[11] & 0x07FF) << 1);
  buf_[17] = static_cast<uint8_t>((data_.ch[11] & 0x07FF) >> 7  | (data_.ch[12] & 0x07FF) << 4);
  buf_[18] = static_cast<uint8_t>((data_.ch[12] & 0x07FF) >> 4  | (data_.ch[13] & 0x07FF) << 7);
  buf_[19] = static_cast<uint8_t>((data_.ch[13] & 0x07FF) >> 1);
  buf_[20] = static_cast<uint8_t>((data_.ch[13] & 0x07FF) >> 9  | (data_.ch[14] & 0x07FF) << 2);
  buf_[21] = static_cast<uint8_t>((data_.ch[14] & 0x07FF) >> 6  | (data_.ch[15] & 0x07FF) << 5);
  buf_[22] = static_cast<uint8_t>((data_.ch[15] & 0x07FF) >> 3);
  buf_[23] = 0x00 | (data_.ch17 * CH17_MASK_) | (data_.ch18 * CH18_MASK_) | (data_.failsafe * FAILSAFE_MASK_) | (data_.lost_frame * LOST_FRAME_MASK_);
  buf_[24] = FOOTER_;
}

void initSBUS()
{
  Serial2.begin(100000, SERIAL_8E2, RXD2, TXD2, 1);
  
  data_.lost_frame = 0;
  data_.failsafe = 0;
  data_.ch17 = 0;
  data_.ch18 = 0;
  for (int i = 0; i < data_.NUM_CH; i++)
  {
    data_.ch[i] = 990; // data values range from (probably) 172 to 1811, 990 is 0ish
  }
  sbus_to_buf(data_, buf);

  timer = timerBegin(0, 80, true);
  timerAttachInterrupt(timer, &onTimer, true);
  timerAlarmWrite(timer, 7000, true); //7 mS interval, defined by sbus. On alarm create task to write data to SBUS_TX
  timerAlarmEnable(timer);
}

#endif
