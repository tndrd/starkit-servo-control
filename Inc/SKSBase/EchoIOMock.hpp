#pragma once

#include "SKSBase/MsgDump.hpp"
#include <bitset>
#include <cassert>
#include <cstring>
#include <iostream>
#include <stdexcept>

namespace StarkitServo::Testing {
struct EchoIOMock {
private:
  int CmdId = 0;
  bool Verbose;

  CsMessageOut MsgOut;

public:
  EchoIOMock(bool verbose = false);

  void Write(const uint8_t *buf, uint8_t size);
  void Read(uint8_t *buf, uint8_t size);
};
} // namespace StarkitServo::Testing
