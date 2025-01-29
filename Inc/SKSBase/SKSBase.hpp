#pragma once
#include "CsMessageAdapter.hpp"
#include <array>
#include <functional>
#include <stdexcept>

namespace StarkitServo {

class SKSBase {
private:
  CsMessageOut TxMessage{};
  std::array<uint8_t, Limits::MaxMsgSize> RxBuffer{};

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

      static Buffers Serialize(SKSBase &ctx, const Request &request);
      static Responce Deserialize(RxFrame &frame);
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

      static Buffers Serialize(SKSBase &ctx, const Request &request);
      static Responce Deserialize(RxFrame &frame);
    };

    struct Read {
      struct Request {
        uint8_t Id;
        int16_t Index;
      };

      struct Responce {
        int32_t Value;
      };

      static Buffers Serialize(SKSBase &ctx, const Request &request);
      static Responce Deserialize(RxFrame &frame);
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

      static Buffers Serialize(SKSBase &ctx, const Request &request);
      static Responce Deserialize(RxFrame &frame);
    };
  };

private:
  static void ValidateServoId(uint8_t id);

  static void ValidateServoValue(uint16_t val, uint8_t id);

  Buffers MakeBuffers(uint16_t rxSize);
  RxFrame Unpack(uint8_t rxSize) const;

public:

  size_t PromiseBufSize() const;

  // Requires two template parameters:
  // Proc: Procedures::X class
  // IO - generic class that supports signatures
  //  "void IO::Write(const uint8_t* src, size_t sz)"
  //  "void IO::Read(uint8_t* dst, size_t sz)"
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

  SKSBase() = default;

  SKSBase(const SKSBase &) = default;
  SKSBase &operator=(const SKSBase &) = default;

  SKSBase(SKSBase &&) = default;
  SKSBase &operator=(SKSBase &&) = default;
};
} // namespace StarkitServo