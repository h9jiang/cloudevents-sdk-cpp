#ifndef CLOUDEVENTSCPPSDK_V1_BINDING_HTTPBINDER_H
#define CLOUDEVENTSCPPSDK_V1_BINDING_HTTPBINDER_H

#include <boost/beast/http.hpp>
#include "v1/protocol_binding/binder.h"

namespace cloudevents {
namespace binding {

/*
 * Template specialization to handle HTTPRequest and HTTPResponse.
 * For the full template code and comments,
 * view `//v1/protocol_binding/binder.h`.
 *
 * uses Boost beast's HTTP Request/ Response representation.
 */

// _____ Operations used in Unbind Structured _____
template <>
absl::StatusOr<std::string> 
  Binder<boost::beast::http::request<boost::beast::http::string_body>>::
  GetContentType(
  const boost::beast::http::request<boost::beast::http::string_body>& http_req);

template <>
absl::StatusOr<std::string> 
  Binder<boost::beast::http::request<boost::beast::http::string_body>>::
  GetPayload(
  const boost::beast::http::request<boost::beast::http::string_body>& http_req);

// _____ Operations used in Unbind Binary _____

// template <>
// absl::StatusOr<io::cloudevents::v1::CloudEvent> 
//   Binder<boost::beast::http::request<boost::beast::http::string_body>>::UnbindBinary(
//   boost::beast::http::request<boost::beast::http::string_body>& http_req);

template <>
absl::Status
  Binder<boost::beast::http::request<boost::beast::http::string_body>>::
  UnbindMetadata(
  const boost::beast::http::request<boost::beast::http::string_body>& http_req,
  io::cloudevents::v1::CloudEvent& cloud_event);

template <>
absl::Status
  Binder<boost::beast::http::request<boost::beast::http::string_body>>::
  UnbindData(
  const boost::beast::http::request<boost::beast::http::string_body>& http_req,
  io::cloudevents::v1::CloudEvent& cloud_event);

// _____ Operations used in Bind Structured _____

template <>
absl::Status 
  Binder<boost::beast::http::request<boost::beast::http::string_body>>::
  BindContentType(const std::string& contenttype,
  boost::beast::http::request<boost::beast::http::string_body>& http_req);

template <>
absl::Status 
  Binder<boost::beast::http::request<boost::beast::http::string_body>>::
  BindDataStructured(const std::string& payload,
  boost::beast::http::request<boost::beast::http::string_body>& http_req);

// _____ Operations used in Bind Binary _____
template <>
absl::Status 
  Binder<boost::beast::http::request<boost::beast::http::string_body>>::
  BindMetadata(const std::string& key, 
  const io::cloudevents::v1::CloudEvent_CloudEventAttribute& val,
  boost::beast::http::request<boost::beast::http::string_body>& http_req);

template <>
absl::Status 
  Binder<boost::beast::http::request<boost::beast::http::string_body>>::
  BindDataBinary(const std::string& bin_data,
  boost::beast::http::request<boost::beast::http::string_body>& http_req);

template <>
absl::Status 
  Binder<boost::beast::http::request<boost::beast::http::string_body>>::
  BindDataText(const std::string& text_data,
  boost::beast::http::request<boost::beast::http::string_body>& http_req);

// template <>
// absl::StatusOr<boost::beast::http::request<boost::beast::http::string_body>> 
//   Binder<boost::beast::http::request<boost::beast::http::string_body>>::BindBinary(
//   io::cloudevents::v1::CloudEvent& cloud_event);

} // binding
} // cloudevents

#endif // CLOUDEVENTSCPPSDK_V1_BINDING_HTTPBINDER_H
