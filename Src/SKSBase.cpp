#include "SKSBase/SKSBase.hpp"

namespace StarkitServo {

using P = SKSBase::Procedures;

auto P::Control::Serialize(SKSBase &ctx, const Request &request) -> Buffers {
  ValidateServoId(request.Id);
  ValidateServoValue(request.Value, request.Id);

  ctx.TxMessage.makeQueryControl(request.Id, request.Value);
  return ctx.MakeBuffers(RxSizes::Control);
}

auto P::Control::Deserialize(RxFrame &frame) -> Responce {
  auto &msg = frame.Msg;

  int16_t value = msg.getInt16();
  int16_t torque = msg.getInt16();

  return {value, torque};
}

auto P::Info::Serialize(SKSBase &ctx, const Request &request) -> Buffers {
  ValidateServoId(request.Id);
  ctx.TxMessage.makeQueryInfo(request.Id);
  return ctx.MakeBuffers(RxSizes::Info);
}

auto P::Info::Deserialize(RxFrame &frame) -> Responce {
  auto &msg = frame.Msg;

  int16_t p1 = msg.getInt16();
  int16_t p2 = msg.getInt16();
  int16_t p3 = msg.getInt16();

  return {p1, p2, p3};
}

auto P::Read::Serialize(SKSBase &ctx, const Request &request) -> Buffers {
  ValidateServoId(request.Id);
  ctx.TxMessage.makeQueryRead(request.Id, request.Index);
  return ctx.MakeBuffers(RxSizes::Read);
}

auto P::Read::Deserialize(RxFrame &frame) -> Responce {
  return {frame.Msg.getInt32()};
}

auto P::Write::Serialize(SKSBase &ctx, const Request &request) -> Buffers {
  ValidateServoId(request.Id);
  ctx.TxMessage.makeQueryWrite(request.Id, request.Index, request.Value);
  return ctx.MakeBuffers(RxSizes::Write);
}

auto P::Write::Deserialize(RxFrame &frame) -> Responce {
  return {frame.Msg.getInt32()};
}

void SKSBase::ValidateServoId(uint8_t id) {
  if (id < Limits::ServoIdMin || id > Limits::ServoIdMax)
    throw std::runtime_error("Servo Id " + std::to_string(id) +
                             " is outside limits");
}

void SKSBase::ValidateServoValue(uint16_t val, uint8_t id) {
  if (val < Limits::ServoValueMin || val > Limits::ServoValueMax)
    throw std::runtime_error("Servo value " + std::to_string(val) +
                             " for servo #" + std::to_string(id) +
                             " is invalid");
}

auto SKSBase::MakeBuffers(uint16_t rxSize) -> Buffers {
  Buffers buffers;

  buffers.TxBuf = reinterpret_cast<const uint8_t *>(TxMessage.buffer());
  buffers.TxSize = TxMessage.length();

  buffers.RxBuf = RxBuffer.data();
  buffers.RxSize = rxSize;

  return buffers;
}

auto SKSBase::Unpack(uint8_t rxSize) const -> RxFrame {
  const char *buf = reinterpret_cast<const char *>(RxBuffer.data());

  auto msg = CsMessageIn{buf, 0, rxSize, 0};

  /*
  std::cout << "Buffer of size " << +rxSize << std::endl;
  for (int i = 0; i < 16; ++i)
    std::cout << std::hex << +RxBuffer[i] << " ";
  */

  uint8_t crc1 = CsMessageOut::crc(buf, rxSize - 1) | 0x80; // Im out rn
  uint8_t crc2 = RxBuffer[rxSize - 1];

  // std::cout << std::endl << "real: " << +crc1 << " vs " << +crc2 <<
  // std::endl; std::cout << std::dec;

  return {msg, crc1 == crc2};
}

size_t SKSBase::PromiseBufSize() const {
    return Limits::MaxMsgSize;
}

} // namespace StarkitServo