#pragma once

#include <sstream>
#include <aws/s3/model/Object.h>
#include <aws/core/utils/DateTime.h>

namespace awscratch { namespace s3 {

class PrettyObject {
 public:
  using Object = Aws::S3::Model::Object;
  using DateFormat = Aws::Utils::DateFormat;
 protected:
  const Object& object_;
 public:
  PrettyObject(const Object &object): object_(object){}
  void pprintTo(std::ostream& oss) const {
    oss << "Object : '" << object_.GetKey() << "'";
    oss << std::endl;
    oss << "\t" << "Last Modified: '" << object_.GetLastModified().ToGmtString(DateFormat::ISO_8601) << "'";
    oss << std::endl;
    oss << "\t" << "Size: '" << object_.GetSize() << "'";
    oss << std::endl;
    // oss << "\t" << "Owner: '" << object_.GetOwner() << "'";
    // oss << std::endl;
    oss << "\t" << "ETag: '" << object_.GetETag() << "'";
    oss << std::endl;
    oss << "--------";
  }
};

}} // awscratch::s3

// std::ostream& operator<<(std::ostream& oss,
//     const awscratch::s3::PrettyObject& object) {
//   object.printTo(oss);
//   return oss;
// }
