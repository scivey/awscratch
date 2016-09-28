#pragma once
#include <string>
#include <vector>
#include <aws/ec2/model/Filter.h>
#include <aws/ec2/model/Instance.h>

namespace awscratch { namespace ec2 {

template<typename TString = std::string,
  template<class...> class TVector = std::vector,
  template<class> class TAllocator = std::allocator>
struct ec2_type_policy {
  using string_t = TString;
  using filter_t = Aws::EC2::Model::Filter;
  using instance_t = Aws::EC2::Model::Instance;
  using filter_vector_t = TVector<filter_t, TAllocator<filter_t>>;
  using string_vector_t = TVector<string_t, TAllocator<string_t>>;
  using instance_vector_t = TVector<instance_t, TAllocator<instance_t>>;
};


}} // awscratch::ec2
