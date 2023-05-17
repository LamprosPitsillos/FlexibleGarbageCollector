#pragma once
#include "Collected.hpp"
#include <type_traits>

/*
TODO:
IMPLEMENT PRIMITIVE WRAPPERS

WE COULD AVOID REPEATING CODE BY WRITING A GENERIC DEFINITION AS A MACRO
i.e. 
#define primitive_wrapper_definition(Type) \
class Type final : public Collected { \
    Type wrappee; \
    ... \
};

THE PROBLEM WITH THIS IS THAT THE NAMES OF THE COLLECTED TYPES WOULD CONFLICT WITH THE ACTUAL
PRIMITIVE TYPES IF "using namespace GC" WERE TO BE USED.
WE COULD JUST NAME OUR WRAPPER CLASSES "class GC_##Type" (example: GC_int)HOWEVER THAT WOULD MAKE IT LESS CLEAN IMO.

THINK ABOUT IT AND LMK 
*/

namespace GC {
    namespace {
    template <typename Type>
    concept IsFundamental = std::is_fundamental<Type>::value;    
    template<IsFundamental Wrappee>
    class CollectedPrimitive final : public Collected {

    };
    }
    using Int = CollectedPrimitive<int>;
    using Long = CollectedPrimitive<long>;
    using Unsigned = CollectedPrimitive<unsigned>;
    using Char = CollectedPrimitive<char>;
    using Float = CollectedPrimitive<float>;
    using Double = CollectedPrimitive<double>;
}
