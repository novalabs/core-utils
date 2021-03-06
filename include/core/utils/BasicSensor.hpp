/* COPYRIGHT (c) 2016-2018 Nova Labs SRL
 *
 * All rights reserved. All use of this software and documentation is
 * subject to the License Agreement located in the file LICENSE.
 */

#pragma once

#include <core/utils/namespace.hpp>

NAMESPACE_CORE_UTILS_BEGIN

template <typename _T>
class BasicSensor
{
public:
    using DataType = _T;

    virtual bool
    init() = 0;

    virtual bool
    configure() = 0;

    virtual bool
    start() = 0;

    virtual bool
    stop() = 0;

    virtual bool
    waitUntilReady() = 0;

    virtual bool
    update() = 0;

    virtual void
    get(
        DataType& data
    ) = 0;

    virtual void
	reset() = 0;
};

NAMESPACE_CORE_UTILS_END
