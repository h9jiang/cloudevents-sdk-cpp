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

} // binding
} // cloudevents
