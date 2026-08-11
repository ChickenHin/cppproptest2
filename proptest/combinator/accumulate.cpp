#include "proptest/combinator/accumulate.hpp"
#include "proptest/combinator/combinatorimpl.hpp"

namespace proptest {
namespace util {

GeneratorCommon accumulateImpl(Function1<ShrinkableBase> gen1, Function1<Function1<ShrinkableBase>> gen2gen, size_t minSize, size_t maxSize)
{
    auto intervalGen = gen::interval<uint64_t>(minSize, maxSize);

    auto deriveAccumulate = [gen1, gen2gen](const uint64_t& size) {
        return Function1<ShrinkableBase>([gen1, gen2gen, size](Random& rand) {
            ShrinkableBase shr = gen1.callDirect(rand);
            for (size_t i = 0; i < size; i++) {
                Function1<ShrinkableBase> gen2 = gen2gen(shr.getAny());
                shr = gen2.callDirect(rand);
            }
            return shr;
        });
    };

    return deriveImpl(intervalGen, [deriveAccumulate](const Any& sizeAny) -> Function1<ShrinkableBase> {
        return deriveAccumulate(sizeAny.getRef<uint64_t>());
    });
}

} // namespace util
} // namespace proptest
