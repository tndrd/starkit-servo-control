#include "SKSBase/EchoIOMock.hpp"

namespace StarkitServo::Testing {

EchoIOMock::EchoIOMock(bool verbose) : Verbose{verbose} {}

void EchoIOMock::Write(const uint8_t *buf, uint8_t size) {
  auto msgIn = CsMessageIn{reinterpret_cast<const char *>(buf), 0, size, 1};
  if (!msgIn.checkCrc(size))
    throw std::runtime_error("Bad CS");

  if (Verbose)
    DumpMsg(msgIn);

  CsMessageOut msg;

  switch (msgIn.cmd()) {

  case Cmds::Control: {
    int16_t val = msgIn.getInt16();
    msg.makeAnswerControl(val, val);
    break;
  }

  case Cmds::Info: {
    int16_t val = msgIn.id();
    msg.makeAnswerInfo(val, val, val);
    break;
  }

  case Cmds::Read: {
    int32_t val = msgIn.getInt16();
    msg.makeAnswerRead(val);
    break;
  }

  case Cmds::Write: {
    msgIn.getInt16();
    int32_t val = msgIn.getInt32();
    msg.makeAnswerWrite(val);
    break;
  }

  default:
    throw std::runtime_error("Unexpected cmd " + std::to_string(CmdId));
  }

  std::memcpy(&MsgOut, &msg, sizeof(msg));
}

void EchoIOMock::Read(uint8_t *buf, uint8_t size) {
  if (MsgOut.length() != size)
    throw std::runtime_error("responce differs from its expected size (" +
                             std::to_string(MsgOut.length()) +
                             " != " + std::to_string(size) + ")");

  std::memcpy(buf, const_cast<char *>(MsgOut.buffer()), size);
}

} // namespace StarkitServo::Testing
