#include "http_binder.h"
#include "pubsub_binder.h"

#include <gtest/gtest.h>

namespace cloudevents {
namespace binding {
using ::cloudevents_absl::StatusOr;
using ::cloudevents_base64::base64_encode;
using ::google::pubsub::v1::PubsubMessage;
using ::io::cloudevents::v1::CloudEvent;
using ::io::cloudevents::v1::CloudEvent_CloudEventAttribute;
using ::cloudevents::format::StructuredCloudEvent;
using ::cloudevents::format::Format;
using ::cloudevents::format::Formatter;
using ::cloudevents::formatter_util::FormatterUtil;
using ::boost::beast::http::string_body;

typedef boost::beast::http::request<string_body> HttpRequest;

TEST(BindUnbind, PubsubToHttpReq) {
  PubsubMessage pubsub_msg;
  (*pubsub_msg.mutable_attributes())["ce-id"] = "1";
  (*pubsub_msg.mutable_attributes())["ce-source"] = "2";
  (*pubsub_msg.mutable_attributes())["ce-spec_version"] = "3";
  (*pubsub_msg.mutable_attributes())["ce-type"] = "4";
  PubsubBinder binder_pubsub;
  HttpReqBinder binder_http_req;

  StatusOr<CloudEvent> unbind = binder_pubsub.Unbind(pubsub_msg);
  
  ASSERT_TRUE(unbind.ok());
  ASSERT_EQ((*unbind).id(), "1");
  ASSERT_EQ((*unbind).source(), "2");
  ASSERT_EQ((*unbind).spec_version(), "3");
  ASSERT_EQ((*unbind).type(), "4");
  
  StatusOr<HttpRequest> bind = binder_http_req.Bind(*unbind);

  ASSERT_TRUE(bind.ok());
  ASSERT_EQ((*bind).base()["ce-id"], "1");
  ASSERT_EQ((*bind).base()["ce-source"], "2");
  ASSERT_EQ((*bind).base()["ce-spec_version"], "3");
  ASSERT_EQ((*bind).base()["ce-type"], "4");
}


}  // namespace binding
}  // namespace cloudevents
