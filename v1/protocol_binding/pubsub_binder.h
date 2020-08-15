#ifndef CLOUDEVENTSCPPSDK_V1_BINDING_PUBSUBBINDER_H
#define CLOUDEVENTSCPPSDK_V1_BINDING_PUBSUBBINDER_H

#include "external/googleapis/google/pubsub/v1/pubsub.pb.h"
#include "v1/protocol_binding/binder.h"

namespace cloudevents {
namespace binding {

/*
 * Template specialization to handle PubsubMessages.
 * For the full template code and comments,
 * view `//v1/protocol_binding/binder.h`.
 */

// _____ Specializations for Unbind Structured _____

template <>
absl::StatusOr<std::string> 
  Binder<google::pubsub::v1::PubsubMessage>::GetContentType(
  const google::pubsub::v1::PubsubMessage& pubsub_msg);

template <>
absl::StatusOr<std::string> 
  Binder<google::pubsub::v1::PubsubMessage>::GetPayload(
  const google::pubsub::v1::PubsubMessage& pubsub_msg);

// _____ Specializations for Unbind Binary _____

template <>
absl::Status Binder<google::pubsub::v1::PubsubMessage>::UnbindMetadata(
  const google::pubsub::v1::PubsubMessage& pubsub_msg, 
  io::cloudevents::v1::CloudEvent& cloud_event);

template <>
absl::Status Binder<google::pubsub::v1::PubsubMessage>::UnbindData(
  const google::pubsub::v1::PubsubMessage& pubsub_msg, 
  io::cloudevents::v1::CloudEvent& cloud_event);

// _____ Specializations for Bind Structured _____

template <>
absl::Status Binder<google::pubsub::v1::PubsubMessage>::BindContentType(
  const std::string& contenttype, google::pubsub::v1::PubsubMessage& pubsub_msg);

template <>
absl::Status Binder<google::pubsub::v1::PubsubMessage>::BindDataStructured(
  const std::string& payload, google::pubsub::v1::PubsubMessage& pubsub_msg);

// _____ Specializations for Bind Binary _____

template <>
absl::Status Binder<google::pubsub::v1::PubsubMessage>::BindMetadata(
  const std::string& key,
  const io::cloudevents::v1::CloudEvent_CloudEventAttribute& val,
  google::pubsub::v1::PubsubMessage& pubsub_msg);

template <>
absl::Status Binder<google::pubsub::v1::PubsubMessage>::BindDataBinary(
  const std::string& bin_data, google::pubsub::v1::PubsubMessage& pubsub_msg);

template <>
absl::Status Binder<google::pubsub::v1::PubsubMessage>::BindDataText(
  const std::string& text_data, google::pubsub::v1::PubsubMessage& pubsub_msg);

} // binding
} // cloudevents

#endif // CLOUDEVENTSCPPSDK_V1_BINDING_PUBSUBBINDER_H
