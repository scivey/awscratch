#pragma once
#include <vector>
#include <string>
#include <aws/ec2/model/Filter.h>
#include "awscratch/ec2/ec2_type_policy.h"

namespace awscratch { namespace ec2 {

template<typename TPolicy = ec2_type_policy<>>
struct filter_factory {
  using policy_t = TPolicy;
  using filter_t = typename policy_t::filter_t;
  using filter_vector_t = typename policy_t::filter_vector_t;
  using string_t = typename policy_t::string_t;

  template<typename T1, typename T2 = T1>
  static filter_t makeFilter(const T1& key, const T2& val) {
    filter_t aFilter;
    aFilter.SetName(key);
    aFilter.AddValues(val);
    return aFilter;
  }

  template<typename T1, typename TValueSequence,
    typename = decltype(std::declval<filter_t>().SetValues(std::declval<TValueSequence>()))>
  static filter_t makeFilter(const T1& key, const TValueSequence& vals) {
    Aws::EC2::Model::Filter aFilter;
    aFilter.SetName(key);
    aFilter.SetValues(vals);
    return aFilter;
  }

  template<typename T1, typename T2 = T1>
  static filter_t makeTagFilter(const T1& key, const T2 &val) {
    typename policy_t::string_t keyBuff = "tag:";
    keyBuff += key;
    return makeFilter(keyBuff, val);
  }


  template<typename TPairs>
  static filter_vector_t makeTagFilter(const TPairs &pairs) {
    filter_vector_t filters;
    for (const auto &onePair: pairs) {
      filters.push_back(makeTagFilter(onePair.first, onePair.second));
    }
    return filters;
  }

  template<typename T1 = typename policy_t::string_t,
    typename T2 = typename policy_t::string_t>
  static filter_vector_t makeTagFilter(std::initializer_list<std::pair<T1, T2>>&& pairs) {
    filter_vector_t filters;
    for (const auto &onePair: pairs) {
      filters.push_back(makeTagFilter(onePair.first, onePair.second));
    }
    return filters;
  }

  template<typename T1 = typename policy_t::string_t,
    typename T2 = typename policy_t::string_t>
  static filter_vector_t makeTagFilter(std::initializer_list<std::pair<const char*, const char*>>&& pairs) {
    filter_vector_t filters;
    for (const auto &onePair: pairs) {
      filters.push_back(makeTagFilter(onePair.first, onePair.second));
    }
    return filters;
  }
};


}}