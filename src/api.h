#ifndef YARP_ADDONS_API_H
#define YARP_ADDONS_API_H

#include <yarp/conf/api.h>
#ifndef YARP_ADDONS_API
#  ifdef YARP_ADDONS_EXPORTS
#    define YARP_ADDONS_API YARP_EXPORT
#    define YARP_ADDONS_EXTERN YARP_EXPORT_EXTERN
#  else
#    define YARP_ADDONS_API YARP_IMPORT
#    define YARP_ADDONS_EXTERN YARP_IMPORT_EXTERN
#  endif
#endif

#endif // YARP_DEV_API_H
