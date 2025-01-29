#include "SKSBase/EchoIOMock.hpp"

namespace StarkitServo::Testing {

EchoIOMock::EchoIOMock(bool verbose) : Verbose{verbose} {}

void EchoIOMock::Synchronize(const uint8_t *reqBuf, uint8_t reqSz, uint8_t *rspBuf,
                 uint8_t rspSz) {
  auto msgIn =
      CsMessageIn{reinterpret_cast<const char *>(reqBuf), 0, reqSz, 1};
  if (!msgIn.checkCrc(reqSz))
    throw std::runtime_error("Bad CS");

  if (Verbose)
    DumpMsg(msgIn);

  CsMessageOut msgOut;

  switch (msgIn.cmd()) {

  case Cmds::Control: {
    int16_t val = msgIn.getInt16();
    msgOut.makeAnswerControl(val, val);
    break;
  }

  case Cmds::Info: {
    int16_t val = msgIn.id();
    msgOut.makeAnswerInfo(val, val, val);
    break;
  }

  case Cmds::Read: {
    int32_t val = msgIn.getInt16();
    msgOut.makeAnswerRead(val);
    break;
  }

  case Cmds::Write: {
    msgIn.getInt16();
    int32_t val = msgIn.getInt32();
    msgOut.makeAnswerWrite(val);
    break;
  }

  default:
    throw std::runtime_error("Unexpected cmd " + std::to_string(msgIn.cmd()));
  }

  if (msgOut.length() != rspSz)
    throw std::runtime_error(
        "Responce' size differs from its expected value (" +
        std::to_string(msgOut.length()) + " != " + std::to_string(rspSz) + ")");

  std::memcpy(rspBuf, const_cast<char *>(msgOut.buffer()), rspSz);
}

} // namespace StarkitServo::Testing
