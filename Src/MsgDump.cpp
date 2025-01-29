#include "SKSBase/MsgDump.hpp"

namespace StarkitServo::Testing {

const char *Cmd2Str(int cmd) {
  switch (cmd) {

#define CASE(cmdname)                                                          \
  case Cmds::cmdname:                                                          \
    return #cmdname

    CASE(Control);
    CASE(Info);
    CASE(Read);
    CASE(Write);

  default:
    return "UNSUPPORTED CMD";
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
  case Cmds::Control:
    DumpControlMsg(msg);
    break;
  case Cmds::Info:
    break;
  case Cmds::Read:
    DumpReadMsg(msg);
    break;
  case Cmds::Write:
    DumpWriteMsg(msg);
    break;

  default:
    std::cout << "UNSUPPORTED_CMD" << std::endl;
  }
};

} // namespace StarkitServo::Testing