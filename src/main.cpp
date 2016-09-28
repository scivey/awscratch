#include <glog/logging.h>
#include <chrono>
#include <thread>
#include <folly/Demangle.h>
#include <folly/FBString.h>
#include <folly/FBVector.h>

#include <typeinfo>

#include <aws/s3/S3Client.h>
#include <aws/s3/model/ListObjectsV2Request.h>
#include <aws/s3/model/ListObjectsV2Result.h>
#include <aws/s3/model/GetObjectRequest.h>
#include <aws/s3/model/GetObjectResult.h>
#include <aws/elasticache/ElastiCacheClient.h>
#include <aws/elasticache/model/DescribeCacheClustersResult.h>
#include <aws/elasticache/model/DescribeCacheClustersRequest.h>
#include <aws/ec2/model/DescribeInstancesRequest.h>
#include <aws/ec2/model/DescribeInstancesResponse.h>
#include <aws/ec2/model/DescribeTagsRequest.h>
#include <aws/ec2/model/Filter.h>

#include <aws/ec2/EC2Client.h>
#include <aws/core/utils/DateTime.h>
#include <aws/core/Aws.h>
#include "awscratch/macros.h"
#include "awscratch/traits.h"
#include "awscratch/ec2/filter_factory.h"
#include "awscratch/ec2/FriendlySyncEC2Client.h"
#include "awscratch/s3/PrettyBucket.h"
#include "awscratch/s3/PrettyObject.h"
#include "awscratch/s3/FriendlySyncS3Client.h"
#include "awscratch/elasticache/PrettyCacheCluster.h"
#include "awscratch/elasticache/FriendlySyncElasticacheClient.h"

#include "awscratch/APIGuard.h"
#include "awscratch/prettyprint/enable.h"

using namespace std;

using namespace awscratch;
using namespace awscratch::s3;
using namespace awscratch::elasticache;
using namespace awscratch::ec2;

void doListBuckets() {
  auto client = FriendlySyncS3Client<>::createShared();
  auto names = client->listBucketNames();
  for (auto &name: names) {
    LOG(INFO) << "BUCKET: '" << name << "'";
  }
}

void doListObjects(const std::string& bucketName) {
  auto client = FriendlySyncS3Client<>::createShared();
  auto objects = client->listObjects(bucketName);
  for (const auto &obj: objects) {
    LOG(INFO) << PrettyObject(obj);
  }
}

void doGetObject(const std::string& bucketName, const std::string& key) {
  auto client = FriendlySyncS3Client<>::createShared();
  auto body = client->getObjectContents(bucketName, key);
  LOG(INFO) << "contents of '" << key << "' : '" << body << "'";
}


void doDescribeElastic(const std::string& clusterId) {
  auto client = FriendlySyncElasticacheClient<>::createShared();
  auto endpoints = client->getEndpointsByClusterId(clusterId);
  for (auto& endpoint: endpoints) {
    LOG(INFO) << "'" << clusterId << "' endpoint: '" << endpoint << "'";
  }
}


void doDescribeEc2() {
  auto client = FriendlySyncEC2Client<>::createShared();
  auto instances = client->describeInstancesByTags({
    {"Role", "worker"}
  });
  for (auto& instance: instances) {
    LOG(INFO) << instance.GetInstanceId() << "\t:\t" << instance.GetPublicDnsName();
  }
}

int main() {
  google::InstallFailureSignalHandler();
  LOG(INFO) << "start";
  APIGuardFactory factory;
  factory.getOptions().loggingOptions.logLevel = Aws::Utils::Logging::LogLevel::Debug;
  auto guard = factory.build();
  doListBuckets();
  doDescribeEc2();
  LOG(INFO) << "end";
}
