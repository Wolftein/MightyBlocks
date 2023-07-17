#pragma once

#include <pplx/pplxtasks.h>

namespace MightyBlocks
{
    /**
     * Basic interface for a CRUD repository.
     */
    template<class Type>
    class Repository
    {
    public:

        virtual pplx::task<Type> Create(Type& object) = 0;
        virtual pplx::task<bool> Update(Type& object) = 0;
        virtual pplx::task<Type> Read(uint64_t id) = 0;
        virtual pplx::task<bool> Delete(uint64_t id) = 0;

    protected:

        /**
         * NOTE: This is required as some SQL library prefer to throw exception instead of using error codes...
         */
        template<typename J, typename T>
        inline auto SafeWrap(T task) -> pplx::task<J>
        {
            return pplx::task<J>([=]() {
                try
                {
                    return task();
                } catch (...)   // TODO: Propagate error on caller?
                {
                    // TODO: Log?
                    return J { };
                }
            });
        }
    };
}