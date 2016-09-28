#include "awscratch/util/util.h"
#include <string>
#include <sstream>
#include <streambuf>
#include <aws/core/utils/DateTime.h>

using namespace std;
using Aws::Utils::DateFormat;
using Aws::Utils::DateTime;

namespace awscratch { namespace util {

string toISO8601(const DateTime& dt) {
  return dt.ToGmtString(DateFormat::ISO_8601);
}

}}