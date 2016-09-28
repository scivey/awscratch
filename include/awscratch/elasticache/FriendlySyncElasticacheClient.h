#pragma once
#include <memory>
#include <aws/elasticache/ElastiCacheClient.h>
#include <sstream>
#include <streambuf>
#include <glog/logging.h>
#include "awscratch/elasticache/elasticache_type_policy.h"
#include "awscratch/macros.h"

namespace awscratch { namespace elasticache {

template<typename TypePolicy = elasticache_type_policy<>>
class FriendlySyncElasticacheClient {
 public:
  using policy_t = TypePolicy;
  using client_t = Aws::ElastiCache::ElastiCacheClient;
  using string_t = typename policy_t::string_t;
  using string_vector_t = typename policy_t::string_vector_t;
  using cluster_t = typename policy_t::cluster_t;

 protected:
  std::unique_ptr<client_t> client_ {nullptr};
 public:
  FriendlySyncElasticacheClient(std::unique_ptr<client_t> client)
    : client_(std::move(client)) {}
  static FriendlySyncElasticacheClient create() {
    return FriendlySyncElasticacheClient {
      std::unique_ptr<client_t> {
        new client_t
      }
    };
  }
  static std::shared_ptr<FriendlySyncElasticacheClient> createShared() {
    return std::make_shared<FriendlySyncElasticacheClient>(create());
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

  template<typename TString>
  cluster_t getClusterById(const TString &clusterId) {
    using request_t = Aws::ElastiCache::Model::DescribeCacheClustersRequest;
    request_t request;
    request.SetCacheClusterId(clusterId);
    request.SetShowCacheNodeInfo(true);
    auto outcome = client_->DescribeCacheClusters(request);
    AWSCRATCH_CHECK_OUTCOME(outcome);
    auto clusters = outcome.GetResultWithOwnership().GetCacheClusters();
    CHECK(clusters.size() > 0);
    return clusters[0];
  }

  template<typename TString>
  string_vector_t getEndpointsByClusterId(const TString &clusterId) {
    string_vector_t endpoints;
    auto cluster = getClusterById(clusterId);
    for (const auto& node: cluster.GetCacheNodes()) {
      endpoints.push_back(node.GetEndpoint().GetAddress());
    }
    return endpoints;
  }

};

}} // awscratch::s3