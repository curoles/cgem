//
// https://gcc.gnu.org/onlinedocs/gcc/Diagnostic-Pragmas.html
//
#pragma once

#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)

#define DO_PRAGMA(x) _Pragma (#x)
#define MESSAGE(x)   DO_PRAGMA(message (#x))
#define TODO(x)      DO_PRAGMA(message ("TODO - " #x))


#define CPP_VERSION  DO_PRAGMA(message "C++ " STR(__cplusplus))

#define INLINE_C static inline
