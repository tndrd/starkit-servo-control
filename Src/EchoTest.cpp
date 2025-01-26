#include "Protocol.hpp"
#include "EchoIOMock.hpp"
#include <random>
#include <limits>
#include <cassert>

using namespace StarkitServo;

static uint8_t ID = 0xC; // 12
using P = Protocol::Procedures;

void TestControl(Testing::EchoIOMock& io, Protocol& servo) {
    int16_t val = 0x10ad;
    auto resp = servo.Call<P::Control>(io, {ID, val});

    assert(resp.Torque == val);
    assert(resp.Value == val);
}

void TestInfo(Testing::EchoIOMock& io, Protocol& servo) {
    auto resp = servo.Call<P::Info>(io, {ID});

    assert(resp.Param1 == ID);
    assert(resp.Param2 == ID);
    assert(resp.Param3 == ID);
}

void TestRead(Testing::EchoIOMock& io, Protocol& servo) {
    int16_t ind = 0x1abc;

    auto resp = servo.Call<P::Read>(io, {ID, ind});

    assert(resp.Value = ind);
}

void TestWrite(Testing::EchoIOMock& io, Protocol& servo) {
    int32_t val = 0x1cbd;
    int16_t ind = 0x1aaa;

    auto resp = servo.Call<P::Write>(io, {ID, ind, val});

    assert(resp.Value == val);
}

int main() {
    auto io = Testing::EchoIOMock(true);
    auto servo = Protocol{};
    
    TestControl(io, servo);
    TestInfo(io, servo);
    TestRead(io, servo);
    TestWrite(io, servo);
}