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

constexpr absl::string_view kHttpContentKey = "Content-Type";

template <>
absl::StatusOr<std::string> Binder<HttpRequest>::GetContentType(HttpRequest& http_req) {
    auto iter = http_req.base().find(kHttpContentKey.data());
    if (iter == http_req.base().end()) {
        return std::string("");
    }
    return std::string(iter->value());
}

template <>
absl::StatusOr<std::string> Binder<HttpRequest>::GetPayload(HttpRequest& http_req) {
    return http_req.body();
}

template <>
absl::StatusOr<CloudEvent> Binder<HttpRequest>::UnbindBinary(HttpRequest& http_req) {
    CloudEvent cloud_event;
    for (auto it = http_req.base().begin(); it!=http_req.base().end(); ++it) {
        std::string key;
        std::string header_key = (*it).name_string().to_string();
        if (header_key == kHttpContentKey.data()) {
            key = kContenttypeKey.data();
        } 
        else if (header_key.rfind(kMetadataPrefix.data(), 0) == 0){
            size_t len_prefix = strlen(kMetadataPrefix.data());
            key = header_key.substr(len_prefix, std::string::npos);
        }
        CloudEventsUtil::SetMetadata(cloud_event, key, (*it).value().to_string());
    }

    std::string http_data = http_req.body();
    if (!http_data.empty()) {
        cloud_event.set_binary_data(http_data);
    }
    
    if (!CloudEventsUtil::IsValid(cloud_event)) {
        return absl::InvalidArgumentError("Pubsub Message given does not contain a valid binary Cloud Event");
    }
    return cloud_event;
}

template <>
absl::Status Binder<HttpRequest>::SetContentType(HttpRequest& http_req,
        std::string contenttype) {
    http_req.base().set(kHttpContentKey.data(), contenttype);
    return absl::Status();
}

template <>
absl::Status Binder<HttpRequest>::SetPayload(HttpRequest& http_req,
        std::string payload) {
    http_req.body() = payload;
    return absl::Status();
}

} // binding
} // cloudevents
