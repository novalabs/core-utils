/* COPYRIGHT (c) 2016-2017 Nova Labs SRL
 *
 * All rights reserved. All use of this software and documentation is
 * subject to the License Agreement located in the file LICENSE.
 */

#pragma once

#include <core/utils/namespace.hpp>
#include <core/os/Mutex.hpp>

NAMESPACE_CORE_UTILS_BEGIN

template <typename T, std::size_t N>
class DoubleBuffer
{
public:
    using  value_type = T;
    using  pointer    = T *;
    static constexpr std::size_t size  = N;
    static constexpr std::size_t bytes = N * sizeof(T);

public:
    DoubleBuffer()
    {
        _read  = _a;
        _write = _b;
        _mutex.initialize();
    }

    inline pointer
    read()
    {
        return _read;
    }

    inline pointer
    write()
    {
        return _write;
    }

    inline void
    copyTo(
        pointer to
    )
    {
        memcpy(to, _read, bytes);
    }

    inline void
    copyFrom(
        pointer from
    )
    {
        memcpy(from, _write, bytes);
    }

    inline void
    swap()
    {
        if (_read == _a) {
            _read  = _b;
            _write = _a;
        } else {
            _read  = _a;
            _write = _b;
        }
    }

    inline void
    lock()
    {
        _mutex.acquire();
    }

    inline void
    unlock()
    {
        _mutex.release();
    }

private:
    T  _a[N];
    T  _b[N];
    T* _read;
    T* _write;
    core::os::Mutex _mutex;
}; 

NAMESPACE_CORE_UTILS_END
