/* COPYRIGHT (c) 2016-2017 Nova Labs SRL
 *
 * All rights reserved. All use of this software and documentation is
 * subject to the License Agreement located in the file LICENSE.
 */

#pragma once

#include <core/utils/namespace.hpp>

NAMESPACE_CORE_UTILS_BEGIN

template <typename T, std::size_t N>
class ShiftRegister
{
public:
    ShiftRegister() : _buffer{T()}, _ptr(0) {}

    T
    shift(
        T in
    )
    {
        T out = _buffer[_ptr]; // OUT

        _buffer[_ptr] = in; // IN

        _ptr++; // Advance...

        if (_ptr == N) {
            // And reset if needed
            _ptr = 0;
        }

        return out;
    }

    void
    reset()
    {
        for (std::size_t i = 0; i < N; i++) {
            _buffer[i] = T();
        }

        _ptr = 0;
    }

    void
    reset(
        T x
    )
    {
        for (std::size_t i = 0; i < N; i++) {
            _buffer[i] = x;
        }

        _ptr = 0;
    }

private:
    T _buffer[N];
    std::size_t _ptr;
};

NAMESPACE_CORE_UTILS_END
