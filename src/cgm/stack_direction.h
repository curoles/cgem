/**@file
 * @brief  Find what direction stack grows
 * @author Igor Lesik
 */
#pragma once
#ifndef CGM_STACK_DIRECTION_H_INCLUDED
#define CGM_STACK_DIRECTION_H_INCLUDED

#include "cgem/stack_direction.h"

namespace cgm {

enum class StackDir : int { Up = 1, Down = -1};

/// Find stack direction
///
inline
auto stack_direction() -> StackDir {
    return (cgm__stack_direction() < 0)? StackDir::Down : StackDir::Up;
}

} // namespace cgm

#endif
