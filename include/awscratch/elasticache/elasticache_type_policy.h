#pragma once
#include <string>
#include <vector>
#include <aws/elasticache/model/CacheCluster.h>

namespace awscratch { namespace elasticache {

template<typename TString = std::string,
  template<class...> class TVector = std::vector,
  template<class> class TAllocator = std::allocator>
struct elasticache_type_policy {
  using string_t = TString;
  using string_vector_t = TVector<string_t, TAllocator<string_t>>;
  using cluster_t = Aws::ElastiCache::Model::CacheCluster;
};


}} // awscratch::ec2
