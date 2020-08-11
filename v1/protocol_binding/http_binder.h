#ifndef CLOUDEVENTSCPPSDK_V1_BINDING_HTTPBINDER_H
#define CLOUDEVENTSCPPSDK_V1_BINDING_HTTPBINDER_H

#include <boost/beast/http.hpp>
#include "v1/protocol_binding/binder.h"

namespace cloudevents {
namespace binding {

typedef boost::beast::http::request<boost::beast::http::string_body> HttpRequest;

// HTTP specialization
template <>
absl::StatusOr<std::string> 
    Binder<boost::beast::http::request<boost::beast::http::string_body>>::
    GetContentType(HttpRequest& http_msg);

// template <>
// absl::StatusOr<std::string> 
//     Binder<boost::beast::http::request<boost::beast::http::string_body>>::GetPayload(
//     boost::beast::http::request<boost::beast::http::string_body>& pubsub_msg);

// template <>
// absl::StatusOr<io::cloudevents::v1::CloudEvent> 
//     Binder<boost::beast::http::request<boost::beast::http::string_body>>::UnbindBinary(
//     boost::beast::http::request<boost::beast::http::string_body>& pubsub_msg);

// template <>
// absl::StatusOr<boost::beast::http::request<boost::beast::http::string_body>> 
//     Binder<boost::beast::http::request<boost::beast::http::string_body>>::BindBinary(
//     io::cloudevents::v1::CloudEvent& cloud_event);

// template <>
// absl::Status 
//     Binder<boost::beast::http::request<boost::beast::http::string_body>>::SetContentType(
//     boost::beast::http::request<boost::beast::http::string_body>& message, 
//     std::string contenttype);

// template <>
// absl::Status 
//     Binder<boost::beast::http::request<boost::beast::http::string_body>>::SetPayload(
//     boost::beast::http::request<boost::beast::http::string_body>& message, 
//     std::string payload);

} // binding
} // cloudevents

#endif // CLOUDEVENTSCPPSDK_V1_BINDING_HTTPBINDER_H
