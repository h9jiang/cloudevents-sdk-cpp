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

typedef absl::flat_hash_map<std::string, CloudEvent_CloudEventAttribute> CeAttrMap;

constexpr absl::string_view kPubsubContentKey = "content-type";

template <>
absl::StatusOr<bool> Binder<PubsubMessage>::InStructuredContentMode(PubsubMessage& pubsub_msg) {
    google::protobuf::Map<std::string,std::string> attrs;
    attrs = pubsub_msg.attributes();
    auto ind = attrs.find(kPubsubContentKey.data());
    return (ind != attrs.end() && (ind -> second).rfind(kContenttypePrefix.data(), 0) == 0); 
}


} // binding
} // cloudevents
