#if DISTRO >= HUMBLE

#include <rmw/features.h>

bool rmw_feature_supported(rmw_feature_t feature)
{
  return false;
}

#endif
