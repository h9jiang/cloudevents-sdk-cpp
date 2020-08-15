#include "http_binder.h"

namespace cloudevents {
namespace binding {

using ::io::cloudevents::v1::CloudEvent;
using ::io::cloudevents::v1::CloudEvent_CloudEventAttribute;
using ::cloudevents::format::StructuredCloudEvent;
using ::cloudevents::format::Format;
using ::cloudevents::format::Formatter;
using ::cloudevents::formatter_util::FormatterUtil;
using ::cloudevents::cloudevents_util::CloudEventsUtil;

typedef boost::beast::http::request<boost::beast::http::string_body> HttpRequest;
typedef absl::flat_hash_map<std::string, CloudEvent_CloudEventAttribute> CeAttrMap;

static const std::string kHttpContentKey = "Content-Type";

// _____ Operations used in Unbind Structured _____

template <>
absl::StatusOr<std::string> Binder<HttpRequest>::GetContentType(const HttpRequest& http_req) {
  auto iter = http_req.base().find(kHttpContentKey.data());
  if (iter == http_req.base().end()) {
    return std::string("");
  }
  return std::string(iter->value());
}

template <>
absl::StatusOr<std::string> Binder<HttpRequest>::GetPayload(const HttpRequest& http_req) {
  return http_req.body();
}

// template <>
// absl::StatusOr<CloudEvent> Binder<HttpRequest>::UnbindBinary(HttpRequest& http_req) {
//   CloudEvent cloud_event;
//   for (auto it = http_req.base().begin(); it!=http_req.base().end(); ++it) {
//     std::string key;
//     std::string header_key = (*it).name_string().to_string();
//     if (header_key == kHttpContentKey.data()) {
//       key = kContenttypeKey.data();
//     } 
//     else if (header_key.rfind(kMetadataPrefix.data(), 0) == 0){
//       size_t len_prefix = strlen(kMetadataPrefix.data());
//       key = header_key.substr(len_prefix, std::string::npos);
//     }
//     CloudEventsUtil::SetMetadata(cloud_event, key, (*it).value().to_string());
//   }

//   std::string http_data = http_req.body();
//   if (!http_data.empty()) {
//     cloud_event.set_binary_data(http_data);
//   }
  
//   if (!CloudEventsUtil::IsValid(cloud_event)) {
//     return absl::InvalidArgumentError("Pubsub Message given does not contain a valid binary Cloud Event");
//   }
//   return cloud_event;
// }

// _____ Operations used in Unbind Binary _____

template <>
absl::Status Binder<HttpRequest>::UnbindMetadata(
    const HttpRequest& http_req, CloudEvent& cloud_event) {
  for (auto it = http_req.base().begin(); it!=http_req.base().end(); ++it) {
    std::string header_key = (*it).name_string().to_string();
    std::string header_val = (*it).value().to_string();
    if (header_key == kHttpContentKey) {
      if (auto set_metadata = CloudEventsUtil::SetMetadata(kContenttypeKey,
          header_val, cloud_event); !set_metadata.ok()) {
        return set_metadata;
      }  
    } 
    else if (header_key.rfind(kMetadataPrefix, 0) == 0){
      size_t len_prefix = kMetadataPrefix.length();
      std::string key = header_key.substr(len_prefix, std::string::npos);
      if (auto set_metadata = CloudEventsUtil::SetMetadata(key,
          header_val, cloud_event); !set_metadata.ok()) {
        return set_metadata;
      }  
    }
  }
  return absl::OkStatus();
}

template <>
absl::Status Binder<HttpRequest>::UnbindData(
    const HttpRequest& http_req, CloudEvent& cloud_event) {
  if (!http_req.body().empty()) {
    cloud_event.set_binary_data(http_req.body());
  }
  return absl::OkStatus();
}

// _____ Operations used in Bind Structured _____

template <>
absl::Status Binder<HttpRequest>::BindContentType(
    const std::string& contenttype, HttpRequest& http_req) {
  http_req.base().set(kHttpContentKey.data(), contenttype);
  return absl::OkStatus();
}

template <>
absl::Status Binder<HttpRequest>::BindDataStructured(
    const std::string& payload, HttpRequest& http_req) {
  http_req.body() = payload;
  return absl::OkStatus();
}

// template <>
// absl::StatusOr<HttpRequest> Binder<HttpRequest>::BindBinary(CloudEvent& cloud_event) {
//   if (!CloudEventsUtil::IsValid(cloud_event)) {
//     return absl::InvalidArgumentError("Cloud Event given is not valid.");
//   }

//   HttpRequest http_req;

//   absl::StatusOr<CeAttrMap> attrs;
//   attrs = CloudEventsUtil::GetMetadata(cloud_event);
//   if (!attrs.ok()) {
//     return attrs.status();
//   }

//   for (auto const& attr : (*attrs)) {
//     absl::StatusOr<std::string> val = CloudEventsUtil::ToString(attr.second);
//     if (!val.ok()) {
//       return val.status();
//     }
//     std::string key = k(kMetadataPrefix + attr.first;
//     http_req.base().setkey,*val);
//   }

//   // http binding spec states to use the body should carry the data as is
//   std::string data;
//   switch (cloud_event.data_oneof_case()) {
//     case CloudEvent::DataOneofCase::kBinaryData: {
//       data = cloud_event.binary_data();
//       break;
//     }
//     case CloudEvent::DataOneofCase::kTextData: {
//       data = cloud_event.text_data();
//       break;
//     }
//     case CloudEvent::DataOneofCase::kProtoData: {
//       // TODO (#17): Handle CloudEvent Any in JsonFormatter
//       return absl::UnimplementedError("protobuf::Any not supported yet.");
//     }
//     case CloudEvent::DATA_ONEOF_NOT_SET: {
//       break;
//     }
//   }

//   http_req.body() = data;
//   return http_req;
// }

// _____ Operations used in Bind Binary _____

template <>
absl::Status Binder<HttpRequest>::BindMetadata(const std::string& key,
    const CloudEvent_CloudEventAttribute& val, HttpRequest& http_req) {
  absl::StatusOr<std::string> val_str = CloudEventsUtil::ToString(val);
  if (!val_str.ok()) {
    return val_str.status();
  }
  http_req.base().set(key, *val_str);
  return absl::OkStatus();
}

template <>
absl::Status Binder<HttpRequest>::BindDataBinary(const std::string& bin_data,
    HttpRequest& http_req) {
  // spec states to place data into body as is
  http_req.body() = bin_data;
  return absl::OkStatus();
}

template <>
absl::Status Binder<HttpRequest>::BindDataText(const std::string& text_data,
    HttpRequest& http_req) {
  // spec states to place data into body as is
  http_req.body() = text_data;
  return absl::OkStatus();
}

} // binding
} // cloudevents
