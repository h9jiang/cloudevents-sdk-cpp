#include "pubsub_binder.h"

namespace cloudevents {
namespace binding {

using ::google::pubsub::v1::PubsubMessage;
using ::io::cloudevents::v1::CloudEvent;
using ::io::cloudevents::v1::CloudEvent_CloudEventAttribute;
using ::cloudevents::format::StructuredCloudEvent;
using ::cloudevents::format::Format;
using ::cloudevents::format::Formatter;
using ::cloudevents::formatter_util::FormatterUtil;
using ::cloudevents::cloudevents_util::CloudEventsUtil;

typedef absl::flat_hash_map<std::string, CloudEvent_CloudEventAttribute>
  CeAttrMap;

inline static constexpr char kPubsubContentKey[] = "content-type";


template <>
absl::StatusOr<std::string> Binder<PubsubMessage>::GetContentType(
    const PubsubMessage& pubsub_msg) {
  google::protobuf::Map<std::string, std::string> attrs =
    pubsub_msg.attributes();
  auto ind = attrs.find(kPubsubContentKey);
  if (ind == attrs.end()) {
    return std::string("");
  }
  return ind -> second;
}

template <>
absl::StatusOr<std::string> Binder<PubsubMessage>::GetPayload(
    const PubsubMessage& pubsub_msg) {
  // get payload and base64 decode
  // no need to unpack due to matching return type
  return base64::base64_decode(pubsub_msg.data());
}

template <>
absl::StatusOr<CloudEvent> Binder<PubsubMessage>::UnbindBinary(
    const PubsubMessage& pubsub_msg) {
  CloudEvent cloud_event;

  for (auto const& attr : pubsub_msg.attributes()) {
    std::string key;
    if (attr.first == kPubsubContentKey) {
      key = kContenttypeKey;
    } else if (attr.first.rfind(kMetadataPrefix, 0) == 0){
      size_t len_prefix = strlen(kMetadataPrefix);
      key = attr.first.substr(len_prefix, std::string::npos);
    }
    if (auto set_md = CloudEventsUtil::SetMetadata(key, attr.second,
        cloud_event); !set_md.ok()) {
      return set_md;
    }
  }

  std::string pubsub_data = pubsub_msg.data();
  if (!pubsub_data.empty()) {
    absl::StatusOr<std::string> decoded = base64::base64_decode(pubsub_data);
    if (!decoded.ok()) {
      return decoded.status();
    }
    cloud_event.set_binary_data((*decoded));
  }

  if (auto is_valid = CloudEventsUtil::IsValid(cloud_event); !is_valid.ok()) {
    return is_valid;
  }

  return cloud_event;
}

template <>
absl::Status Binder<PubsubMessage>::SetContentType(
    const std::string& contenttype, PubsubMessage& pubsub_msg) {
  (*pubsub_msg.mutable_attributes())[kPubsubContentKey] = contenttype;
  return absl::Status();
}

template <>
absl::Status Binder<PubsubMessage>::SetPayload(
    const std::string& payload, PubsubMessage& pubsub_msg) {
  pubsub_msg.set_data(payload);
  return absl::Status();
}

template <>
absl::StatusOr<PubsubMessage> Binder<PubsubMessage>::BindBinary(
    const CloudEvent& cloud_event) {
  if (auto is_valid = CloudEventsUtil::IsValid(cloud_event); !is_valid.ok()) {
    return is_valid;
  }

  PubsubMessage pubsub_msg;

  absl::StatusOr<CeAttrMap> attrs;
  attrs = CloudEventsUtil::GetMetadata(cloud_event);
  if (!attrs.ok()) {
    return attrs.status();
  }

  for (auto const& attr : (*attrs)) {
    absl::StatusOr<std::string> val = CloudEventsUtil::ToString(attr.second);
    if (!val.ok()) {
      return val.status();
    }
    std::string key = kMetadataPrefix + attr.first;
    (*pubsub_msg.mutable_attributes())[key] = (*val);
  }

  std::string data;
  switch (cloud_event.data_oneof_case()) {
    // curly braces to prevent cross initialization
    case CloudEvent::DataOneofCase::kBinaryData: {
      // cloud event spec uses base64 encoding for binary data as well
      data = cloud_event.binary_data();
      break;
    }
    case CloudEvent::DataOneofCase::kTextData: {
      absl::StatusOr<std::string> encoded;
      encoded = base64::base64_encode(cloud_event.text_data());
      if (!encoded.ok()) {
        return encoded.status();
      }
      data = (*encoded);
      break;
    }
    case CloudEvent::DataOneofCase::kProtoData: {
      // TODO (#17): Handle CloudEvent Any in JsonFormatter
      return absl::UnimplementedError("protobuf::Any not supported yet.");
    }
    case CloudEvent::DATA_ONEOF_NOT_SET: {
      break;
    }
  }

  pubsub_msg.set_data(data);
  return pubsub_msg;
}

}  // namespace binding
}  // namespace cloudevents
