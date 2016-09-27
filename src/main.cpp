#include <glog/logging.h>
#include <chrono>
#include <thread>
#include <aws/s3/S3Client.h>
#include <aws/s3/model/ListObjectsV2Request.h>
#include <aws/s3/model/ListObjectsV2Result.h>

#include <aws/core/utils/DateTime.h>
#include <aws/core/Aws.h>
#include "awscratch/macros.h"
#include "awscratch/s3/PrettyBucket.h"
#include "awscratch/APIGuard.h"

using namespace std;

using namespace awscratch;
using namespace awscratch::s3;
using Aws::S3::Model::ListObjectsV2Request;

void doListBuckets() {
  Aws::S3::S3Client client;
  auto response = client.ListBuckets();
  AWSCRATCH_CHECK_OUTCOME(response);
  Aws::S3::Model::ListBucketsResult bucketList = response.GetResultWithOwnership();
  auto buckets = bucketList.GetBuckets();
  for (auto &bucket: buckets) {
    LOG(INFO) << PrettyBucket(bucket);
  }
}

void doListObjects() {
  Aws::S3::S3Client client;
  ListObjectsV2Request request;
  request.SetBucket("curata-rethinkdb-backup-alphavpc");
  auto response = client.ListObjectsV2(request);
  AWSCRATCH_CHECK_OUTCOME(response);
  auto objectList = response.GetResultWithOwnership();
  LOG(INFO) << "n keys : " << objectList.GetKeyCount();
  const auto& contents = objectList.GetContents();
  for (const auto &obj: contents) {
    LOG(INFO) << "key: " << obj.GetKey();
  }
}

int main() {
  google::InstallFailureSignalHandler();
  LOG(INFO) << "start";
  APIGuardFactory factory;
  factory.getOptions().loggingOptions.logLevel = Aws::Utils::Logging::LogLevel::Debug;
  auto guard = factory.build();
  doListObjects();
  LOG(INFO) << "end";
}
