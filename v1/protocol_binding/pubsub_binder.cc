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

inline static const std::string kPubsubContenttypeKey = "content-type";

// _____ Specializations for Unbind Structured _____

template <>
cloudevents_absl::StatusOr<std::string> Binder<PubsubMessage>::GetContentType(
    const PubsubMessage& pubsub_msg) {
  google::protobuf::Map<std::string, std::string> attrs =
    pubsub_msg.attributes();
  auto ind = attrs.find(kPubsubContenttypeKey);
  if (ind == attrs.end()) {
    return std::string("");
  }
  return ind -> second;
}

template <>
cloudevents_absl::StatusOr<std::string> Binder<PubsubMessage>::GetPayload(
    const PubsubMessage& pubsub_msg) {
  // get payload and base64 decode
  return cloudevents_base64::base64_decode(pubsub_msg.data());
}

// _____ Specializations for Unbind Binary _____

template <>
absl::Status Binder<PubsubMessage>::UnbindMetadata(
    const PubsubMessage& pubsub_msg, CloudEvent& cloud_event) {
  for (auto const& attr : pubsub_msg.attributes()) {
    std::string key;
    if (attr.first == kPubsubContenttypeKey) {
      key = kContenttypeKey;
    } else if (attr.first.rfind(kMetadataPrefix, 0) == 0){
      size_t len_prefix = kMetadataPrefix.length();
      key = attr.first.substr(len_prefix, std::string::npos);
    }
    if (auto set_md = CloudEventsUtil::SetMetadata(key, attr.second,
        cloud_event); !set_md.ok()) {
      return set_md;
    }
  }
  return absl::OkStatus();
}

template <>
absl::Status Binder<PubsubMessage>::UnbindData(
    const PubsubMessage& pubsub_msg, CloudEvent& cloud_event) {
  // both CloudEvent.binary_data and Pubsub.payload uses base64 encoding
  cloud_event.set_binary_data(pubsub_msg.data());
  return absl::OkStatus();
}

// _____ Specializations for Bind Structured _____

template <>
absl::Status Binder<PubsubMessage>::BindContentType(
    const std::string& contenttype, PubsubMessage& pubsub_msg) {
  (*pubsub_msg.mutable_attributes())[kPubsubContenttypeKey] = contenttype;
  return absl::OkStatus();
}

template <>
absl::Status Binder<PubsubMessage>::BindDataStructured(
    const std::string& payload, PubsubMessage& pubsub_msg) {
  pubsub_msg.set_data(payload);
  return absl::OkStatus();
}

// _____ Specializations for Bind Binary _____

template <>
absl::Status Binder<PubsubMessage>::BindMetadata(const std::string& key,
    const CloudEvent_CloudEventAttribute& val, PubsubMessage& pubsub_msg) {
  cloudevents_absl::StatusOr<std::string> val_str = CloudEventsUtil::ToString(val);
  if (!val_str.ok()) {
    return val_str.status();
  }
  (*pubsub_msg.mutable_attributes())[key] = *val_str;
  return absl::OkStatus();
}

template <>
absl::Status Binder<PubsubMessage>::BindDataBinary(const std::string& bin_data,
    PubsubMessage& pubsub_msg) {
  // both CloudEvent.data and pubsub_msg.data are base64 encoded
  pubsub_msg.set_data(bin_data);
  return absl::OkStatus();
}

template <>
absl::Status Binder<PubsubMessage>::BindDataText(const std::string& text_data,
    PubsubMessage& pubsub_msg) {
  cloudevents_absl::StatusOr<std::string> encoded = cloudevents_base64::base64_encode(text_data);
  if (!encoded.ok()) {
    return encoded.status();
  }
  pubsub_msg.set_data(*encoded);
  return absl::OkStatus();
}

}  // namespace binding
}  // namespace cloudevents
