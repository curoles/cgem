/**@file
 * @brief  Find what direction stack grows
 * @author Igor Lesik
 */
#pragma once
#ifndef CGEM_STACK_DIRECTION_H_INCLUDED
#define CGEM_STACK_DIRECTION_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @return 1 if up, -1 if down
 */
int cgm__stack_direction();

#ifdef __cplusplus
} // extern "C"
#endif

#endif
