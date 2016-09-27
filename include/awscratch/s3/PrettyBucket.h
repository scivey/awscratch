#pragma once

#include <sstream>
#include <aws/s3/model/Bucket.h>
#include <aws/core/utils/DateTime.h>

namespace awscratch { namespace s3 {

class PrettyBucket {
 public:
  using Bucket = Aws::S3::Model::Bucket;
  using DateFormat = Aws::Utils::DateFormat;
 protected:
  const Bucket& bucket_;
 public:
  PrettyBucket(const Bucket &bucket): bucket_(bucket){}
  void printTo(std::ostream& oss) const {
    oss << "Bucket"
        << "['" << bucket_.GetName() << "']"
        << "  :  "
        << bucket_.GetCreationDate().ToGmtString(DateFormat::ISO_8601);
  }
};

}} // awscratch::s3

std::ostream& operator<<(std::ostream& oss,
    const awscratch::s3::PrettyBucket& bucket) {
  bucket.printTo(oss);
  return oss;
}
