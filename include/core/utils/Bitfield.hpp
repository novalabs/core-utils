/* COPYRIGHT (c) 2016 Nova Labs SRL
 *
 * All rights reserved. All use of this software and documentation is
 * subject to the License Agreement located in the file LICENSE.
 */
#pragma once

#include <core/utils/namespace.hpp>

NAMESPACE_CORE_UTILS_BEGIN

#define BITFIELD_NAMESPACE_DEFINED
#include "bitfield.h"

// Macro to define whole field partial specilaziation of BitFieldMember
#define WHOLE_FIELD(type) \
   template <> \
   struct BitFieldMember<type, 0, sizeof(type)* 8> { \
      using T = type; \
      T value; \
\
      operator T() const \
      { \
         return value; \
      } \
\
      BitFieldMember& \
      operator=( \
         T v \
      ) \
      { \
         value = v; \
         return *this; \
      } \
   };

template <typename T, int Offset, int Bits, typename E>
struct BitFieldEnumMember {
   T value;

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
      return (value >> Offset) & Maximum;
   }

   operator E() const
   {
      return static_cast<E>((value >> Offset) & Maximum);
   }

   BitFieldEnumMember&
   operator=(
      T v
   )
   {
      assert(v <= Maximum);                 // v must fit inside the bitfield member
      value = (value & ~Mask) | (v << Offset);
      return *this;
   }

   BitFieldEnumMember&
   operator=(
      E e
   )
   {
      T v = static_cast<T>(e);

      assert(v <= Maximum);                 // v must fit inside the bitfield member
      value = (value & ~Mask) | (v << Offset);
      return *this;
   }
};

#define ADD_ENUM_BITFIELD_MEMBER(memberName, offset, bits, enumType) \
   NAMESPACE_CORE_UTILS::BitFieldEnumMember<StorageType, offset, bits, enumType>memberName;

#undef ADD_BITFIELD_MEMBER
#define ADD_BITFIELD_MEMBER(memberName, offset, bits) \
   NAMESPACE_CORE_UTILS::BitFieldMember<StorageType, offset, bits>memberName;

#undef ADD_CONST_BITFIELD_MEMBER
#define ADD_CONST_BITFIELD_MEMBER(memberName, offset, bits, value) \
   NAMESPACE_CORE_UTILS::BitFieldConstMember<StorageType, offset, bits, value>memberName;

#undef ADD_BITFIELD_ARRAY
#define ADD_BITFIELD_ARRAY(memberName, offset, bits, numItems) \
   NAMESPACE_CORE_UTILS::BitFieldArray<StorageType, offset, bits, numItems>memberName;

WHOLE_FIELD(uint8_t)

NAMESPACE_CORE_UTILS_END
