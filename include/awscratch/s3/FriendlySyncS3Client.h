#pragma once
#include <memory>
#include <aws/s3/S3Client.h>
#include <sstream>
#include <streambuf>
#include <glog/logging.h>
#include "awscratch/s3/s3_type_policy.h"
#include "awscratch/macros.h"

namespace awscratch { namespace s3 {

template<typename TypePolicy = s3_type_policy<>>
class FriendlySyncS3Client {
 public:
  using policy_t = TypePolicy;
  using client_t = Aws::S3::S3Client;
  using string_t = typename policy_t::string_t;
  using bucket_t = typename policy_t::bucket_t;
  using object_t = typename policy_t::object_t;

  using string_vector_t = typename policy_t::string_vector_t;
  using bucket_vector_t = typename policy_t::bucket_vector_t;
  using object_vector_t = typename policy_t::object_vector_t;

  using get_object_result_t = Aws::S3::Model::GetObjectResult;
  using list_objects_result_t = Aws::S3::Model::ListObjectsV2Result;
  using list_buckets_result_t = Aws::S3::Model::ListBucketsResult;

 protected:
  std::unique_ptr<client_t> client_ {nullptr};
 public:
  FriendlySyncS3Client(std::unique_ptr<client_t> client)
    : client_(std::move(client)) {}
  static FriendlySyncS3Client create() {
    return FriendlySyncS3Client {
      std::unique_ptr<client_t> {
        new client_t
      }
    };
  }
  static std::shared_ptr<FriendlySyncS3Client> createShared() {
    return std::make_shared<FriendlySyncS3Client>(create());
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

  list_buckets_result_t listBucketsRaw() {
    auto outcome = client_->ListBuckets();
    AWSCRATCH_CHECK_OUTCOME(outcome);
    return outcome.GetResultWithOwnership();
  }

  bucket_vector_t listBuckets() {
    auto response = listBucketsRaw();
    bucket_vector_t buckets = response.GetBuckets();
    return buckets;
  }

  string_vector_t listBucketNames() {
    string_vector_t names;
    for (const auto& bucket: listBuckets()) {
      names.push_back(bucket.GetName());
    }
    return names;
  }

  template<typename TString>
  list_objects_result_t listObjectsRaw(const TString& bucketName) {
    using request_t = Aws::S3::Model::ListObjectsV2Request;
    request_t request;
    request.SetBucket(bucketName);
    auto outcome = client_->ListObjectsV2(request);
    AWSCRATCH_CHECK_OUTCOME(outcome);
    return outcome.GetResultWithOwnership();
  }

  template<typename TString>
  object_vector_t listObjects(const TString& bucketName) {
    auto result = listObjectsRaw(bucketName);
    object_vector_t objects = result.GetContents();
    return objects;
  }

  template<typename TString>
  string_vector_t listObjectKeys(const TString& bucketName) {
    string_vector_t keys;
    for (const auto &object: listObjects(bucketName)) {
      keys.push_back(object.GetKey());
    }
    return keys;
  }

  template<typename T1, typename T2 = T1>
  get_object_result_t getObject(const T1& bucketName, const T2& key) {
    using request_t = Aws::S3::Model::GetObjectRequest;
    request_t request;
    request.SetBucket(bucketName);
    request.SetKey(key);
    auto outcome = client_->GetObject(request);
    AWSCRATCH_CHECK_OUTCOME(outcome);
    return outcome.GetResultWithOwnership();
  }

  template<typename T1, typename T2 = T1>
  string_t getObjectContents(const T1& bucketName, const T2& key) {
    auto result = getObject(bucketName, key);
    std::stringbuf contentBuff;
    result.GetBody().get(contentBuff);
    return contentBuff.str();
  }
};

}} // awscratch::s3