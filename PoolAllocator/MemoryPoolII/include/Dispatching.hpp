#pragma once

#include "ContiguousMemoryPool.hpp"
#include "AlignedMemoryPool.hpp"
#include "CudaMemoryPool.hpp"

/**
 * @brief Compile-time trait to check if a memory pool type is a CPU-based pool.
 *
 * Evaluates to `true` if the type `T` is either `AlignedMemoryPool` or `ContiguousMemoryPool`,
 * which are assumed to represent CPU-accessible memory pools.
 *
 * @tparam T The memory pool type to check.
 * @retval true  If `T` is a CPU pool type.
 * @retval false Otherwise.
 */
template<typename T>
constexpr bool is_cpu_pool_v = std::is_same_v<T, AlignedMemoryPool> || std::is_same_v<T, ContiguousMemoryPool>;

/**
 * @brief Compile-time trait to check if a memory pool type is a GPU-based pool.
 *
 * Evaluates to `true` if the type `T` is `CudaMemoryPool`, which represents a CUDA-capable
 * device memory pool.
 *
 * @tparam T The memory pool type to check.
 * @retval true  If `T` is a GPU pool type.
 * @retval false Otherwise.
 */
template<typename T>
constexpr bool is_gpu_pool_v = std::is_same_v<T, CudaMemoryPool>;

/**
 * @brief Invokes the callable using compile-time dispatch based on the tag type.
 *
 * This function performs a call to the callable object's templated call operator,
 * explicitly instantiating it with the tag type `Tag`. It perfectly forwards all arguments
 * to the callable's `operator()<Tag>()` function, preserving their value categories
 * (e.g., lvalue, rvalue) and qualifiers.
 *
 * @tparam Tag     The tag type used to specialize the callable's templated operator().
 * @tparam Args    Variadic template parameter pack for the arguments to forward.
 *
 * @param callable A callable object (e.g., functor or lambda) that provides a templated
 *                 `operator()<Tag>(Args&&...)`.
 * @param args     Arguments to forward to the callable's templated call operator.
 *
 * @note This is the core of compile-time tag dispatching. It enables different behavior
 *       to be selected and compiled based on the type `Tag`, allowing zero-cost abstraction
 *       and inlining of specialized code paths.
 */
template<typename Tag, typename Callable, typename... Args>
void dispatch( Callable && callable, Args && ... args) {
    callable.template operator()<Tag>( std::forward<Args>( args ) ... );
}

/**
 * @brief Dispatches a callable object based on the static type of a tag, forwarding all arguments.
 *
 * This function template acts as a bridge to the compile-time dispatcher by deducing the type `T`
 * from the tag object (typically used for type-based kernel dispatching). It forwards the callable
 * and all provided arguments to the underlying `dispatch<T>()` function.
 *
 * @tparam Tag      The tag type to dispatch on (deduced from the first argument).
 * @tparam Callable A callable object (e.g., functor or lambda) that must have a templated 
 *                  call operator: `template<typename Tag> void operator()(Args&&...)`.
 * @tparam Args     Variadic template parameter pack for the arguments to forward.
 *
 * @param tag       The tag instance used only for type deduction (not used at runtime).
 * @param callable  The callable object that will be invoked with the tag as a template parameter.
 * @param args      Arguments to forward to the callable's `operator()`.
 *
 * @note This function does not copy or move the tag. Arguments are perfectly forwarded to preserve
 *       lvalue/rvalue and const/volatile qualifiers.
 */
template<typename Tag, typename Callable, typename... Args>
void dispatch_from( [[maybe_unused]] Tag const& tag, Callable && callable, Args && ... args ) {
    dispatch<Tag>( std::forward<Callable>( callable ), std::forward<Args>( args )... );
}
