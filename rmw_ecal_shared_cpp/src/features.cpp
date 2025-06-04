#if ROS_DISTRO >= HUMBLE

#include <rmw/features.h>

#include <rmw_ecal_shared_cpp/rmw/rmw.hpp>

bool rmw_feature_supported(rmw_feature_t feature)
{
  return eCAL::rmw::rmw_feature_supported(feature);
}

#endif
