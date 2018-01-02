/* COPYRIGHT (c) 2016-2018 Nova Labs SRL
 *
 * All rights reserved. All use of this software and documentation is
 * subject to the License Agreement located in the file LICENSE.
 */

#pragma once

#include <core/utils/namespace.hpp>

NAMESPACE_CORE_UTILS_BEGIN

template <typename T>
class Accumulator {
    public:
        Accumulator() {reset();};

        void reset() {
            _accumulator = T();
            _counter = 0;
        }

        inline T value() {
            return _accumulator;
        }

        inline std::size_t counter() {
            return _counter;
        }

        template<typename TT>
        void add(TT x);
    private:
        T _accumulator;
        std::size_t _counter;

};

template<typename T>
template<typename TT>
void Accumulator<T>::add(TT x) {
    _accumulator += x;
    _counter++;
}

NAMESPACE_CORE_UTILS_END
