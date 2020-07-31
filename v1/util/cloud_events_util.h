#ifndef CLOUDEVENTSCPPSDK_V1_UTIL_CLOUDEVENTSUTIL
#define CLOUDEVENTSCPPSDK_V1_UTIL_CLOUDEVENTSUTIL

#include "absl/container/flat_hash_map.h"
#include "third_party/statusor/statusor.h"
#include "third_party/base64/base64.h"
#include "proto/cloud_event.pb.h"

namespace cloudevents {
namespace cloudevents_util {

class CloudEventsUtil {
    public:
        // validate if given CloudEvent fulfills requirements to be valid
        static bool IsValid(io::cloudevents::v1::CloudEvent cloud_event);

        // get metadata from CloudEvent in a single map
        static absl::StatusOr<
            absl::flat_hash_map<std::string, io::cloudevents::v1::CloudEvent_CloudEventAttribute>>
        GetMetadata(io::cloudevents::v1::CloudEvent cloud_event);

        // convert CloudEvent Attributes to their canonical string representaiton
        // as per outlined in https://github.com/cloudevents/spec/blob/master/spec.md#type-system
        static absl::StatusOr<std::string> StringifyCeType(io::cloudevents::v1::CloudEvent_CloudEventAttribute attr);    

        // Constexpr keys used when interacting with CloudEvent
        inline static constexpr absl::string_view kCeIdKey = "id";
        inline static constexpr absl::string_view kCeSourceKey = "source";
        inline static constexpr absl::string_view kCeSpecKey = "spec_version";
        inline static constexpr absl::string_view kCeTypeKey = "type";
};

} // util
} // cloudevents

#endif //CLOUDEVENTSCPPSDK_V1_UTIL_CLOUDEVENTSUTIL
