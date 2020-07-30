#include "formatter_util.h"

namespace cloudevents {
namespace util {

using ::cloudevents::format::Format;
using ::cloudevents::format::Formatter;
using ::cloudevents::format::JsonFormatter;

absl::StatusOr<Format> FormatterUtil::DestringifyFormat(std::string format_str) {
    if (format_str == "json") {
        return Format::kJson;
    }
    return absl::InvalidArgumentError("The given format is not currently recognized by the SDK.");
}

} // util
} // cloudevents
