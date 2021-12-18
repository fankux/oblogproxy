/**
 * Copyright (c) 2021 OceanBase
 * OceanBase Migration Service LogProxy is licensed under Mulan PubL v2.
 * You can use this software according to the terms and conditions of the Mulan PubL v2.
 * You may obtain a copy of Mulan PubL v2 at:
 *          http://license.coscl.org.cn/MulanPubL-2.0
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
 * EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
 * MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
 * See the Mulan PubL v2 for more details.
 */

#pragma once

#include "codec/message.h"
#include "codec/msg_buf.h"

namespace google {
namespace protobuf {
class Message;
}  // namespace protobuf
}  // namespace google

namespace oceanbase {
namespace logproxy {

class MessageEncoder {
public:
  virtual ~MessageEncoder() = default;

  virtual int encode(const Message& msg, MsgBuf& buffer) = 0;
};

class LegacyEncoder : public MessageEncoder {
  OMS_AVOID_COPY(LegacyEncoder);

public:
  static LegacyEncoder& instance()
  {
    static LegacyEncoder singleton;
    return singleton;
  }

private:
  LegacyEncoder();

public:
  int encode(const Message& msg, MsgBuf& buffer) override;

private:
  std::unordered_map<int8_t, std::function<int(const Message&, MsgBuf&)>> _funcs;
};

class ProtobufEncoder : public MessageEncoder {
  OMS_AVOID_COPY(ProtobufEncoder);
  OMS_SINGLETON(ProtobufEncoder);

public:
  int encode(const Message& msg, MsgBuf& buffer) override;

private:
  static int encode_message(const google::protobuf::Message& pb_msg, MessageType type, MsgBuf& buffer, bool magic);

  static int encode_error_response(const Message& msg, MsgBuf& buffer);

  static int encode_client_handshake_request(const Message& msg, MsgBuf& buffer);

  static int encode_client_handshake_response(const Message& msg, MsgBuf& buffer);

  static int encode_runtime_status(const Message& msg, MsgBuf& buffer);

  static int encode_data_client(const Message& msg, MsgBuf& buffer);
};

}  // namespace logproxy
}  // namespace oceanbase
