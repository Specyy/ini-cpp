#ifndef INICPP_CONFIG_H
#define INICPP_CONFIG_H 1

#ifdef INICPP_DLL
#   ifdef INICPP_BUILD
#       ifdef _WIN32
#           define INICPP __declspec(dllexport)
#       elif defined (__GNUC__)
#           define INICPP __attribute__((visibility("default")))
#       else
#           define INICPP
#       endif
#   else
#       ifdef _WIN32
#           define INICPP __declspec(dllimport)
#       else
#           define INICPP 
#       endif
#   endif
#else
#   define INICPP
#endif
#endif