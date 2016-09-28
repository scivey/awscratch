#pragma once

#include <sstream>
#include <aws/elasticache/model/CacheCluster.h>
#include <aws/core/utils/DateTime.h>
#include "awscratch/util/util.h"

namespace awscratch { namespace elasticache {

class PrettyCacheCluster {
 public:
  using CacheCluster = Aws::ElastiCache::Model::CacheCluster;
 protected:
  const CacheCluster& cluster_;
 public:
  PrettyCacheCluster(const CacheCluster &cluster): cluster_(cluster){}
  void pprintTo(std::ostream& oss) const {
    oss << "CacheCluster: '" << cluster_.GetCacheClusterId() << "'"
        << std::endl
        << "\tEngine: '" << cluster_.GetEngine() << "'"
        << std::endl
        << "\tEngine Version: '" << cluster_.GetEngineVersion() << "'"
        << std::endl
        << "\tCreation Time: '"
            << util::toISO8601(cluster_.GetCacheClusterCreateTime())
            << "'"
        << std::endl
        << "--------";
  }
};

}} // awscratch::s3

// std::ostream& operator<<(std::ostream& oss,
//     const awscratch::elasticache::PrettyCacheCluster& cluster) {
//   cluster.printTo(oss);
//   return oss;
// }
