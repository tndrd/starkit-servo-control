#pragma once

#include "SKSBase/CsMessageAdapter.hpp"
#include <iostream>

namespace StarkitServo::Testing {
const char *Cmd2Str(int cmd);

void DumpControlMsg(CsMessageIn msg);
void DumpWriteMsg(CsMessageIn msg);
void DumpReadMsg(CsMessageIn msg);
void DumpMsg(CsMessageIn msg);
} // namespace StarkitServo::Testing