#include "http_binder.h"

#include <gtest/gtest.h>

namespace cloudevents {
namespace binding {
using ::io::cloudevents::v1::CloudEvent;
using ::cloudevents::format::StructuredCloudEvent;
using ::cloudevents::format::Format;
using ::cloudevents::format::Formatter;
using ::cloudevents::formatter_util::FormatterUtil;

typedef boost::beast::http::request<boost::beast::http::string_body> HttpRequest;

TEST(Bind, Invalid) {
    absl::StatusOr<HttpRequest> bind;
    Binder<HttpRequest> binder;
    CloudEvent ce;
    bind = binder.Bind(ce);

    ASSERT_FALSE(bind.ok());
    ASSERT_TRUE(absl::IsInvalidArgument(bind.status()));
}

TEST(Bind, Binary_Required) {
    absl::StatusOr<HttpRequest> bind;
    Binder<HttpRequest> binder;
    CloudEvent ce;
    ce.set_id("1");
    ce.set_source("2");
    ce.set_spec_version("3");
    ce.set_type("4");

    bind = binder.Bind(ce);

    ASSERT_TRUE(bind.ok());
    ASSERT_EQ((*bind).base()["ce-id"], "1");
    ASSERT_EQ((*bind).base()["ce-source"], "2");
    ASSERT_EQ((*bind).base()["ce-spec_version"], "3");
    ASSERT_EQ((*bind).base()["ce-type"], "4");
}

TEST(Bind, Structured_Required) {
    absl::StatusOr<HttpRequest> bind;
    Binder<HttpRequest> binder;
    CloudEvent ce;
    ce.set_id("1");
    ce.set_source("2");
    ce.set_spec_version("3");
    ce.set_type("4");

    bind = binder.Bind(ce, Format::kJson);

    ASSERT_TRUE(bind.ok());
    ASSERT_EQ((*bind).base()["content-type"], "application/cloudevents+json");
    ASSERT_EQ((*bind).body(), "{\n\t\"id\" : \"1\",\n\t\"source\" : \"2\",\n\t\"spec_version\" : \"3\",\n\t\"type\" : \"4\"\n}");
}

TEST(Unbind, Binary_Required) {
    absl::StatusOr<CloudEvent> unbind;
    Binder<HttpRequest> binder;
    HttpRequest http_req;
    http_req.base().set("ce-id", "1");
    http_req.base().set("ce-source", "2");
    http_req.base().set("ce-spec_version", "3");
    http_req.base().set("ce-type", "4");

    unbind = binder.Unbind(http_req);

    ASSERT_TRUE(unbind.ok());
    ASSERT_EQ((*unbind).id(), "1");
    ASSERT_EQ((*unbind).source(), "2");
    ASSERT_EQ((*unbind).spec_version(), "3");
    ASSERT_EQ((*unbind).type(), "4");
}

TEST(Unbind, Structured_Required) {
    absl::StatusOr<CloudEvent> unbind;
    Binder<HttpRequest> binder;
    HttpRequest http_req;
    http_req.base().set("content-type", "application/cloudevents+json");
    http_req.body() = "{\n\t\"id\" : \"1\",\n\t\"source\" : \"/test\",\n\t\"spec_version\" : \"1.0\",\n\t\"type\" : \"test\"\n}";
    unbind = binder.Unbind(http_req);
    ASSERT_TRUE(unbind.ok());
    ASSERT_EQ((*unbind).id(), "1");
    ASSERT_EQ((*unbind).source(), "/test");
    ASSERT_EQ((*unbind).spec_version(), "1.0");
    ASSERT_EQ((*unbind).type(), "test");
}

} // binding
} // cloudevents
