#pragma once
#include <string>
#include <vector>
#include <aws/s3/model/Bucket.h>
#include <aws/s3/model/Object.h>
#include <aws/s3/model/GetObjectResult.h>


namespace awscratch { namespace s3 {

template<typename TString = std::string,
  template<class...> class TVector = std::vector,
  template<class> class TAllocator = std::allocator>
struct s3_type_policy {
  using string_t = TString;
  using bucket_t = Aws::S3::Model::Bucket;
  using object_t = Aws::S3::Model::Object;
  using object_result_t = Aws::S3::Model::GetObjectResult;
  using string_vector_t = TVector<string_t, TAllocator<string_t>>;
  using bucket_vector_t = TVector<bucket_t, TAllocator<bucket_t>>;
  using object_vector_t = TVector<object_t, TAllocator<object_t>>;
};


}} // awscratch::ec2
