#pragma once
#include <memory>
#include <aws/ec2/EC2Client.h>
#include <glog/logging.h>
#include "awscratch/ec2/ec2_type_policy.h"
#include "awscratch/ec2/filter_factory.h"
#include "awscratch/macros.h"

namespace awscratch { namespace ec2 {

template<typename TypePolicy = ec2_type_policy<>>
class FriendlySyncEC2Client {
 public:
  using policy_t = TypePolicy;
  using client_t = Aws::EC2::EC2Client;
  using filter_factory_t = filter_factory<policy_t>;
  using instance_vector_t = typename policy_t::instance_vector_t;
  using string_t = typename policy_t::string_t;

 protected:
  std::unique_ptr<client_t> client_ {nullptr};
 public:
  FriendlySyncEC2Client(std::unique_ptr<client_t> client)
    : client_(std::move(client)) {}
  static FriendlySyncEC2Client create() {
    return FriendlySyncEC2Client {
      std::unique_ptr<client_t> {
        new client_t
      }
    };
  }
  static std::shared_ptr<FriendlySyncEC2Client> createShared() {
    return std::make_shared<FriendlySyncEC2Client>(create());
  }
  bool isValid() const {
    return !!client_;
  }
  operator bool() const {
    return isValid();
  }
  client_t* getUnderlyingClient() {
    DCHECK(isValid());
    return client_.get();
  }

  static instance_vector_t getInstancesFromDescribeInstancesOutcome(
      Aws::EC2::Model::DescribeInstancesOutcome&& outcome) {
    AWSCRATCH_CHECK_OUTCOME(outcome);
    auto result = outcome.GetResultWithOwnership();
    instance_vector_t instances;
    for (const auto &reservation: result.GetReservations()) {
      for (const auto &instance: reservation.GetInstances()) {
        instances.push_back(instance);
      }
    }
    return instances;
  }

  template<typename TTags>
  instance_vector_t describeInstancesByTags(TTags&& tags) {
    using req_t = Aws::EC2::Model::DescribeInstancesRequest;
    req_t request;
    auto tagFilter = filter_factory_t::makeTagFilter(tags);
    request.SetFilters(tagFilter);
    return getInstancesFromDescribeInstancesOutcome(client_->DescribeInstances(request));
  }

  template<typename TTags>
  instance_vector_t describeInstancesByTags(const TTags& tags) {
    using req_t = Aws::EC2::Model::DescribeInstancesRequest;
    req_t request;
    auto tagFilter = filter_factory_t::makeTagFilter(tags);
    request.SetFilters(tagFilter);
    return getInstancesFromDescribeInstancesOutcome(client_->DescribeInstances(request));
  }

  instance_vector_t describeInstancesByTags(
      std::initializer_list<std::pair<const char*, const char*>>&& tags) {
    using req_t = Aws::EC2::Model::DescribeInstancesRequest;
    req_t request;
    auto tagFilter = filter_factory_t::makeTagFilter(tags);
    request.SetFilters(tagFilter);
    return getInstancesFromDescribeInstancesOutcome(client_->DescribeInstances(request));
  }

  typename policy_t::instance_vector_t describeInstances() {
    using req_t = Aws::EC2::Model::DescribeInstancesRequest;
    req_t request;
    return getInstancesFromDescribeInstancesOutcome(client_->DescribeInstances(request));
  }

};

}} // awscratch::ec2