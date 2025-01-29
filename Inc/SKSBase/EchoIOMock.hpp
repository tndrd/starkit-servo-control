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
  bool Verbose;

public:
  EchoIOMock(bool verbose = false);
  void Synchronize(const uint8_t *reqBuf, uint8_t reqSz, uint8_t *rspBuf, uint8_t rspSz);
};
} // namespace StarkitServo::Testing
