/* COPYRIGHT (c) 2016-2017 Nova Labs SRL
 *
 * All rights reserved. All use of this software and documentation is
 * subject to the License Agreement located in the file LICENSE.
 */

// Derived from original work from https://github.com/preshing/cpp11-on-multicore

// Original Copyright follows
/*
//---------------------------------------------------------
Copyright (c) 2015 Jeff Preshing

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not
   claim that you wrote the original software. If you use this software
   in a product, an acknowledgement in the product documentation would be
   appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be
   misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
//---------------------------------------------------------
*/

#ifndef __CPP11OM_BITFIELD_H__
#define __CPP11OM_BITFIELD_H__

#include <cassert>

// Added by Nova Labs
#ifndef BITFIELD_NAMESPACE_DEFINED
#error "Please do not include bitfield.h directly"
#endif

#ifdef assert
#undef assert
#define assert(x)
#endif

//---------------------------------------------------------
// BitFieldMember<>: Used internally by ADD_BITFIELD_MEMBER macro.
// All members are public to simplify compliance with sections 9.0.7 and
// 9.5.1 of the C++11 standard, thereby avoiding undefined behavior.
//---------------------------------------------------------
template <typename T, int Offset, int Bits>
struct BitFieldMember
{
    T value;

    static_assert(Offset + Bits <= (int) sizeof(T) * 8, "Member exceeds bitfield boundaries");
    static_assert(Bits < (int) sizeof(T) * 8, "Can't fill entire bitfield with one member");

    static const T Maximum = (T(1) << Bits) - 1;
    static const T Mask = Maximum << Offset;
    T maximum() const { return Maximum; }
    T one() const { return T(1) << Offset; }

    operator T() const
    {
        return (value >> Offset) & Maximum;
    }

    BitFieldMember& operator=(T v)
    {
        assert(v <= Maximum);               // v must fit inside the bitfield member
        value = (value & ~Mask) | (v << Offset);
        return *this;
    }

    BitFieldMember& operator+=(T v)
    {
        assert(T(*this) + v <= Maximum);    // result must fit inside the bitfield member
        value += v << Offset;
        return *this;
    }

    BitFieldMember& operator-=(T v)
    {
        assert(T(*this) >= v);              // result must not underflow
        value -= v << Offset;
        return *this;
    }

    BitFieldMember& operator++() { return *this += 1; }
    BitFieldMember operator++(int)        // postfix form
    {
        BitFieldMember tmp(*this);
        operator++();
        return tmp;
    }
    BitFieldMember& operator--() { return *this -= 1; }
    BitFieldMember operator--(int)       // postfix form
    {
        BitFieldMember tmp(*this);
        operator--();
        return tmp;
    }
};


//---------------------------------------------------------
// BitFieldArray<>: Used internally by ADD_BITFIELD_ARRAY macro.
// All members are public to simplify compliance with sections 9.0.7 and
// 9.5.1 of the C++11 standard, thereby avoiding undefined behavior.
//---------------------------------------------------------
template <typename T, int BaseOffset, int BitsPerItem, int NumItems>
struct BitFieldArray
{
    T value;

    static_assert(BaseOffset + BitsPerItem * NumItems <= (int) sizeof(T) * 8, "Array exceeds bitfield boundaries");
    static_assert(BitsPerItem < (int) sizeof(T) * 8, "Can't fill entire bitfield with one array element");

    static const T Maximum = (T(1) << BitsPerItem) - 1;
    T maximum() const { return Maximum; }
    int numItems() const { return NumItems; }

    class Element
    {
    private:
        T& value;
        int offset;

    public:
        Element(T& value, int offset) : value(value), offset(offset) {}
        T mask() const { return Maximum << offset; }

        operator T() const
        {
            return (value >> offset) & Maximum;
        }

        Element& operator=(T v)
        {
            assert(v <= Maximum);               // v must fit inside the bitfield member
            value = (value & ~mask()) | (v << offset);
            return *this;
        }

        Element& operator+=(T v)
        {
            assert(T(*this) + v <= Maximum);    // result must fit inside the bitfield member
            value += v << offset;
            return *this;
        }

        Element& operator-=(T v)
        {
            assert(T(*this) >= v);               // result must not underflow
            value -= v << offset;
            return *this;
        }

        Element& operator++() { return *this += 1; }
        Element operator++(int)                // postfix form
        {
	    Element tmp(*this);
	    operator++();
	    return tmp;
	}
        Element& operator--() { return *this -= 1; }
        Element operator--(int)                // postfix form
	{
	    Element tmp(*this);
	    operator--();
	    return tmp;
	}
    };

    Element operator[](int i)
    {
        assert(i >= 0 && i < NumItems);     // array index must be in range
        return Element(value, BaseOffset + BitsPerItem * i);
    }

    const Element operator[](int i) const
    {
        assert(i >= 0 && i < NumItems);     // array index must be in range
        return Element(value, BaseOffset + BitsPerItem * i);
    }
};

// Added by Nova Labs
template <typename T, int Offset, int Bits, T Value>
struct BitFieldConstMember {
   static_assert(Offset + Bits <= (int)sizeof(T) * 8, "Member exceeds bitfield boundaries");
   static_assert(Bits < (int)sizeof(T) * 8, "Can't fill entire bitfield with one member");

   static const T Maximum = (T(1) << Bits) - 1;
   static const T Mask    = Maximum << Offset;
   T
   maximum() const
   {
      return Maximum;
   }

   T
   one() const
   {
      return T(1) << Offset;
   }

   operator T() const
   {
      return (Value >> Offset) & Maximum;
   }
};

//---------------------------------------------------------
// Bitfield definition macros.
// For usage examples, see RWLock and LockReducedDiningPhilosophers.
// All members are public to simplify compliance with sections 9.0.7 and
// 9.5.1 of the C++11 standard, thereby avoiding undefined behavior.
//---------------------------------------------------------
/*! \brief Bitfield definition macro
 */
#define BEGIN_BITFIELD_TYPE(typeName, T) \
    union typeName \
    { \
        struct Wrapper { T value; }; \
        Wrapper wrapper; \
        typeName(T v = 0) { wrapper.value = v; } \
        typeName& operator=(T v) { wrapper.value = v; return *this; } \
        operator T&() { return wrapper.value; } \
        operator T() const { return wrapper.value; } \
        typedef T StorageType; //!< Storage type

/*! \brief Bitfield member definition macro
 */
#define ADD_BITFIELD_MEMBER(memberName, offset, bits) \
        BitFieldMember<StorageType, offset, bits> memberName;

// Added by Nova Labs
/*! \brief Bitfield const member definition macro
 */
#define ADD_CONST_BITFIELD_MEMBER(memberName, offset, bits, value) \
		BitFieldConstMember<StorageType, offset, bits, value>  memberName;

#define ADD_BITFIELD_ARRAY(memberName, offset, bits, numItems) \
        BitFieldArray<StorageType, offset, bits, numItems> memberName;

/*! \brief Bitfield member definition macro
 */
#define END_BITFIELD_TYPE() \
    };


#endif // __CPP11OM_BITFIELD_H__

