// CsMessage.h adapter
// The purpose of this file is namespace cleanup.
// Undeffing unused macro constants, transferring the required
// ones into enum classes

#pragma once
#include "CsMessage/CsMessage.h"
#include <iostream>

namespace StarkitServo {
struct Limits {
  static constexpr int ServoIdMin = 0;
  static constexpr int ServoIdMax = 15;

  static constexpr int ServoValueMin = CS_ANGLE_MIN;
  static constexpr int ServoValueMax = CS_ANGLE_MAX;
};

struct RxSizes {
  static constexpr uint8_t Header = 1;
  static constexpr uint8_t Tail = 1;

  static constexpr uint8_t Control = 2 + 2;
  static constexpr uint8_t Info = 3 * 2;
  static constexpr uint8_t Write = 4;
  static constexpr uint8_t Read = 4;
};

struct Cmds {
  static constexpr uint8_t Control = CS_CMD_MSG_CONTROL;
  static constexpr uint8_t Info = CS_CMD_MSG_INFO;
  static constexpr uint8_t Write = CS_CMD_MSG_WRITE;
  static constexpr uint8_t Read = CS_CMD_MSG_READ;
};

namespace Testing {
const char *Cmd2Str(int cmd) {
  switch (cmd) {
#define CASE(cmdname)                                                          \
  case cmdname:                                                                \
    return #cmdname
    CASE(CS_CMD_MSG_CONTROL);
    CASE(CS_CMD_MSG_CONTROL_EX);
    CASE(CS_CMD_MSG_INFO);
    CASE(CS_CMD_MSG_READ);
    CASE(CS_CMD_MSG_WRITE);
    CASE(CS_CMD_MSG_FLASH);

  default:
    return "UNKNOWN_CMD";
  }
#undef CASE
}

void DumpControlMsg(CsMessageIn msg) {
  int16_t value = msg.getInt16();
  std::cout << "Value: " << std::hex << value << std::dec << std::endl;
}

void DumpWriteMsg(CsMessageIn msg) {
  int index = msg.getUInt16();
  std::cout << "Index: " << index << std::endl;
  std::cout << "Value: " << msg.getInt32() << std::endl;
}

void DumpReadMsg(CsMessageIn msg) {
  int index = msg.getUInt16();
  std::cout << "Index: " << index << std::endl;
}

// Need to make a copy here
void DumpMsg(CsMessageIn msg) {
  std::cout << "id:  " << msg.id() << std::endl;

  int cmd = msg.cmd();

  std::cout << "cmd: " << Cmd2Str(cmd) << std::endl;

  switch (cmd) {
  case CS_CMD_MSG_CONTROL:
    DumpControlMsg(msg);
    break;
  case CS_CMD_MSG_INFO:
    break;
  case CS_CMD_MSG_READ:
    DumpReadMsg(msg);
    break;
  case CS_CMD_MSG_WRITE:
    DumpWriteMsg(msg);
    break;

  default:
    std::cout << "Unexpected message" << std::endl;
  }
};

} // namespace Testing

} // namespace StarkitServo

#undef CS_MESSAGE_VERSION
#undef CS_CMD_MSG_CONTROL
#undef CS_CMD_MSG_INFO
#undef CS_CMD_MSG_CONTROL_EX
#undef CS_CMD_MSG_WRITE
#undef CS_CMD_MSG_READ
#undef CS_CMD_MSG_FLASH
#undef CS_CMD_FLASH_LENGTH
#undef CS_CMD_LENGHTS
#undef CS_SIGNATURE_CONFIG
#undef CS_SIGNATURE_MFLASH
#undef CS_SIGNATURE_MOTOR
#undef CS_SIGNATURE_LMOTOR
#undef CS_SIGNATURE_TENSO
#undef CS_SIGNATURE_FORCE
#undef CS_SIGNATURE_HOLL
#undef CS_UE_NONE
#undef CS_UE_SWITCH
#undef CS_UE_ERASE
#undef CS_UE_FLASH
#undef CS_CB_SIGNATURE
#undef CS_CB_VERSION
#undef CS_CB_DEVICE_ID
#undef CS_CB_PROTOCOL_ID
#undef CS_CB_DEVICE_MODE
#undef CS_CB_UART_ZUBR_BASE
#undef CS_CB_BAUDRATE
#undef CS_CB_START_PROG
#undef CS_CB_INIT_CONFIG
#undef CS_CB_RESET_MODE
#undef CS_CB_SET_PROTOCOL
#undef CS_CB_ERASE_PROG
#undef CS_CB_TIME_SEC_LOW
#undef CS_CB_X
#undef CS_CB_Y
#undef CS_CB_Z
#undef CS_CB_W
#undef CS_CB_ACC
#undef CS_CB_HOLL_CH0
#undef CS_CB_HOLL_CH1
#undef CS_CB_HOLL_CH2
#undef CS_CB_ANGLE_BASE
#undef CS_CB_ADC_BASE
#undef CS_CB_TARGET_PWM
#undef CS_CB_CONTROL_VALUE
#undef CS_CB_RANGLE_PID_BASE
#undef CS_CB_RANGLE_VELO_BASE
#undef CS_CB_RLIGHT_VELO_BASE
#undef CS_CB_RLIGHT_SPRING_BASE
#undef CS_CB_RMOMENT_VELO_BASE
#undef CS_CB_RMOMENT_FRIC_BASE
#undef CS_CB_RCALIBR_VELO_BASE
#undef CS_CB_RCALIBR_PID_BASE
#undef CS_CB_RANGLE_T_BASE
#undef CS_CB_RPID_PID_BASE
#undef CS_CB_RPID_VELO_BASE
#undef CS_CB_FORCE_TOP
#undef CS_CB_FORCE_BOT
#undef CS_CB_FORCE_TOP_MIN
#undef CS_CB_FORCE_TOP_MAX
#undef CS_UM_FREE
#undef CS_UM_HOLD
#undef CS_UM_SOFT
#undef CS_UM_NONE
#undef CS_UM_SET_ZERO
#undef CS_UM_ZERO_UNLOCK
#undef CS_UM_ZP_UNLOCK
#undef CS_UM_ZP_LOCK
#undef CS_UM_ZP_SET_FACTORY
#undef CS_UM_ZP_SET_NULL
#undef CS_UM_ZP_SET_BEGIN
#undef CS_UM_ZP_SET_END
#undef CS_ANGLE_OFFSET
#undef CS_ANGLE_MIN
#undef CS_ANGLE_MAX
#undef CS_PWM_CENTRAL