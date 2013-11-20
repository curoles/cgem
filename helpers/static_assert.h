#pragma once
#ifndef STATIC_ASSERT

#define STATIC_ASSERT(...) \
static_assert(__VA_ARGS__, #__VA_ARGS__)

#endif
