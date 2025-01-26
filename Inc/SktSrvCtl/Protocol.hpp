#pragma once
#include "CsMessageAdapter.hpp"
#include <array>
#include <functional>
#include <stdexcept>

namespace StarkitServo {

class Protocol {
private:
  CsMessageOut TxMessage{};
  std::array<uint8_t, 256> RxBuffer{};

public:
  struct Buffers {
    const uint8_t *TxBuf = nullptr;
    uint8_t *RxBuf = nullptr;

    uint8_t TxSize = 0;
    uint8_t RxSize = 0;
  };

  struct RxFrame {
    CsMessageIn Msg;
    bool Ok;
  };

  struct Procedures {
    struct Control {
      struct Request {
        uint8_t Id;
        int16_t Value;
      };

      struct Responce {
        int16_t Value = 0;
        int16_t Torque = 0;
      };

      static Buffers Serialize(Protocol &ctx, const Request &request) {
        ValidateServoId(request.Id);
        ValidateServoValue(request.Value, request.Id);

        ctx.TxMessage.makeQueryControl(request.Id, request.Value);
        return ctx.MakeBuffers(RxSizes::Control);
      }

      static Responce Deserialize(RxFrame &frame) {
        auto &msg = frame.Msg;

        int16_t value = msg.getInt16();
        int16_t torque = msg.getInt16();

        return {value, torque};
      }
    };

    struct Info {
      struct Request {
        uint8_t Id;
      };

      struct Responce {
        int16_t Param1 = 0;
        int16_t Param2 = 0;
        int16_t Param3 = 0;
      };

      static Buffers Serialize(Protocol &ctx, const Request &request) {
        ValidateServoId(request.Id);
        ctx.TxMessage.makeQueryInfo(request.Id);
        return ctx.MakeBuffers(RxSizes::Info);
      }

      static Responce Deserialize(RxFrame &frame) {
        auto &msg = frame.Msg;

        int16_t p1 = msg.getInt16();
        int16_t p2 = msg.getInt16();
        int16_t p3 = msg.getInt16();

        return {p1, p2, p3};
      }
    };

    struct Read {
      struct Request {
        uint8_t Id;
        int16_t Index;
      };

      struct Responce {
        int32_t Value;
      };

      static Buffers Serialize(Protocol &ctx, const Request &request) {
        ValidateServoId(request.Id);
        ctx.TxMessage.makeQueryRead(request.Id, request.Index);
        return ctx.MakeBuffers(RxSizes::Read);
      }

      static Responce Deserialize(RxFrame &frame) {
        return {frame.Msg.getInt32()};
      }
    };

    struct Write {
      struct Request {
        uint8_t Id;
        int16_t Index;
        int32_t Value;
      };

      struct Responce {
        int32_t Value;
      };

      static Buffers Serialize(Protocol &ctx, const Request &request) {
        ValidateServoId(request.Id);
        ctx.TxMessage.makeQueryWrite(request.Id, request.Index, request.Value);
        return ctx.MakeBuffers(RxSizes::Write);
      }

      static Responce Deserialize(RxFrame &frame) {
        return {frame.Msg.getInt32()};
      }
    };
  };

private:
  static void ValidateServoId(uint8_t id) {
    if (id < Limits::ServoIdMin || id > Limits::ServoIdMax)
      throw std::runtime_error("Servo Id " + std::to_string(id) +
                               " is outside limits");
  }

  static void ValidateServoValue(uint16_t val, uint8_t id) {
    if (val < Limits::ServoValueMin || val > Limits::ServoValueMax)
      throw std::runtime_error("Servo value " + std::to_string(val) +
                               " for servo #" + std::to_string(id) +
                               " is invalid");
  }

  Buffers MakeBuffers(uint16_t rxSize) {
    Buffers buffers;

    buffers.TxBuf = reinterpret_cast<const uint8_t *>(TxMessage.buffer());
    buffers.TxSize = TxMessage.length();

    buffers.RxBuf = RxBuffer.data();
    buffers.RxSize = rxSize;

    return buffers;
  }

  RxFrame Unpack(uint8_t rxSize) {
    const char *buf = reinterpret_cast<const char *>(RxBuffer.data());

    auto msg = CsMessageIn{buf, 0, rxSize, 0};

    /*
    std::cout << "Buffer of size " << +rxSize << std::endl;
    for (int i = 0; i < 16; ++i)
      std::cout << std::hex << +RxBuffer[i] << " ";
    */
    
    uint8_t crc1 = CsMessageOut::crc(buf, rxSize - 1) | 0x80; // Im out rn
    uint8_t crc2 = RxBuffer[rxSize - 1];

    //std::cout << std::endl << "real: " << +crc1 << " vs " << +crc2 << std::endl;
    //std::cout << std::dec;

    return {msg, crc1 == crc2};
  }

public:
  template <typename Proc, typename IO>
  typename Proc::Responce Call(IO &io, const typename Proc::Request &request) {
    TxMessage = {};
    
    auto bufrs = Proc::Serialize(*this, request);
    io.Write(bufrs.TxBuf, bufrs.TxSize);
    
    uint8_t rxSize = RxSizes::Header + bufrs.RxSize + RxSizes::Tail;
    io.Read(bufrs.RxBuf, rxSize);
    auto frame = Unpack(rxSize);

    if (!frame.Ok)
      throw std::runtime_error("Bad control sum");

    return Proc::Deserialize(frame);
  }

  Protocol() = default;

  Protocol(const Protocol &) = default;
  Protocol &operator=(const Protocol &) = default;

  Protocol(Protocol &&) = default;
  Protocol &operator=(Protocol &&) = default;
};
} // namespace StarkitServo