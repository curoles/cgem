/**
 *
 * Resources:
 *  - http://www.drdobbs.com/cpp/mixin-based-programming-in-c/184404445
 *  - http://cgi.di.uoa.gr/~smaragd/practical-fmtd.pdf
 */

#pragma once
#ifndef CGEM_MIXIN_H_INCLUDED
#define CGEM_MIXIN_H_INCLUDED

#define mixin_class(ClassName)\
template<class Super> class ClassName : public Super

#endif

