#ifndef YARP_ADDONS_API_H
#define YARP_ADDONS_API_H

#if defined WIN32
#define YARP_ADDONS_IMPORT __declspec(dllimport)
#define YARP_ADDONS_EXPORT __declspec(dllexport)
#else
#define YARP_ADDONS_IMPORT
#define YARP_ADDONS_EXPORT
#endif


#ifndef YARP_ADDONS_API
#  ifdef YARP_ADDONS_EXPORTS
#    define YARP_ADDONS_API YARP_EXPORT
//#    define YARP_ADDONS_EXTERN YARP_EXPORT_EXTERN
#  else
#    define YARP_ADDONS_API YARP_ADDONS_IMPORT
//#    define YARP_ADDONS_EXTERN YARP_IMPORT_EXTERN
#  endif
#endif

#endif // YARP_DEV_API_H
