// (C) Copyright 2008 CodeRage, LLC (turkanis at coderage dot com)
// (C) Copyright 2003-2007 Jonathan Turkanis
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt.)

// See http://www.boost.org/libs/iostreams for documentation.

#ifndef NDNBOOST_IOSTREAMS_DETAIL_BROKEN_OVERLOAD_RESOLUTION_STREAM_BUFFER_HPP_INCLUDED
#define NDNBOOST_IOSTREAMS_DETAIL_BROKEN_OVERLOAD_RESOLUTION_STREAM_BUFFER_HPP_INCLUDED

#include <ndnboost/iostreams/detail/broken_overload_resolution/forward.hpp>
#include <ndnboost/throw_exception.hpp>

namespace ndnboost { namespace iostreams {

template< typename T, 
          typename Tr = 
              NDNBOOST_IOSTREAMS_CHAR_TRAITS(
                  NDNBOOST_DEDUCED_TYPENAME char_type_of<T>::type 
              ),
          typename Alloc = 
              std::allocator<
                  NDNBOOST_DEDUCED_TYPENAME char_type_of<T>::type 
              >,
          typename Mode = NDNBOOST_DEDUCED_TYPENAME mode_of<T>::type >
class stream_buffer
    : public detail::stream_buffer_traits<T, Tr, Alloc, Mode>::type
{
private:
    NDNBOOST_STATIC_ASSERT((
        is_convertible<
            NDNBOOST_DEDUCED_TYPENAME iostreams::category_of<T>::type, Mode
        >::value
    ));
    typedef typename 
            detail::stream_buffer_traits<
                T, Tr, Alloc, Mode
            >::type                           base_type;
public:
    typedef typename char_type_of<T>::type    char_type;
    struct category 
        : Mode,
          closable_tag,
          streambuf_tag
        { };
    NDNBOOST_IOSTREAMS_STREAMBUF_TYPEDEFS(Tr)
    stream_buffer() { }
    ~stream_buffer()
    { 
        try { 
            if (this->is_open() && this->auto_close()) 
                this->close(); 
        } catch (...) { } 
    }
    template<typename U0>
    stream_buffer(const U0& u0)
    {
        open_impl(detail::forward<T, U0>(), u0);
    }
    template<typename U0, typename U1>
    stream_buffer(const U0& u0, const U1& u1)
    {
        open_impl(detail::forward<T, U0>(), u0, u1);
    }
    template<typename U0, typename U1, typename U2>
    stream_buffer(const U0& u0, const U1& u1, const U2& u2)
    {
        open_impl(detail::forward<T, U0>(), u0, u1, u2);
    }
#if !NDNBOOST_WORKAROUND(NDNBOOST_MSVC, <= 1300) //---------------------------------//
    template<typename U0>
    stream_buffer(U0& u0)
    {
        open_impl(detail::forward<T, U0>(), u0);
    }
    template<typename U0, typename U1>
    stream_buffer(U0& u0, const U1& u1)
    {
        open_impl(detail::forward<T, U0>(), u0, u1);
    }
    template<typename U0, typename U1, typename U2>
    stream_buffer(U0& u0, const U1& u1, const U2& u2)
    {
        open_impl(detail::forward<T, U0>(), u0, u1, u2);
    }
#endif // !NDNBOOST_WORKAROUND(NDNBOOST_MSVC, <= 1300) //---------------------------//
    template<typename U0>
    void open(const U0& u0)
    {
        open_impl(detail::forward<T, U0>(), u0);
    }
    template<typename U0, typename U1>
    void open(const U0& u0, const U1& u1)
    {
        open_impl(detail::forward<T, U0>(), u0, u1);
    }
    template<typename U0, typename U1, typename U2>
    void open(const U0& u0, const U1& u1, const U2& u2)
    {
        open_impl(detail::forward<T, U0>(), u0, u1, u2);
    }
#if !NDNBOOST_WORKAROUND(NDNBOOST_MSVC, <= 1300) //---------------------------------//
    template<typename U0>
    void open(U0& u0)
    {
        open_impl(detail::forward<T, U0>(), u0);
    }
    template<typename U0, typename U1>
    void open(U0& u0, const U1& u1)
    {
        open_impl(detail::forward<T, U0>(), u0, u1);
    }
    template<typename U0, typename U1, typename U2>
    void open(U0& u0, const U1& u1, const U2& u2)
    {
        open_impl(detail::forward<T, U0>(), u0, u1, u2);
    }
#endif // !NDNBOOST_WORKAROUND(NDNBOOST_MSVC, <= 1300) //---------------------------//
    T& operator*() { return *this->component(); }
    T* operator->() { return this->component(); }
private:
    template<typename U0>
    void open_impl(mpl::false_, const U0& u0)
    {
        base_type::open(const_cast<U0&>(u0), -1, -1);
    }
#if !NDNBOOST_WORKAROUND(NDNBOOST_MSVC, <= 1300) //---------------------------------//
    template<typename U0>
    void open_impl(mpl::false_, U0& u0)
    {
        base_type::open(detail::wrap(u0), -1, -1);
    }
#endif // !NDNBOOST_WORKAROUND(NDNBOOST_MSVC, <= 1300) //---------------------------//
    template<typename U0>
    void open_impl(mpl::true_, const U0& u0)
    {
        base_type::open(T(const_cast<U0&>(u0)), -1, -1);
    }
#if !NDNBOOST_WORKAROUND(NDNBOOST_MSVC, <= 1300) //---------------------------------//
    template<typename U0>
    void open_impl(mpl::true_, U0& u0)
    {
        base_type::open(T(u0), -1, -1);
    }
#endif // !NDNBOOST_WORKAROUND(NDNBOOST_MSVC, <= 1300) //---------------------------//
    template<typename U0, typename U1>
    void open_impl(mpl::false_, const U0& u0, const U1& u1)
    {
        base_type::open(u0, u1, -1);
    }
    template<typename U0, typename U1>
    void open_impl(mpl::true_, const U0& u0, const U1& u1)
    {
        base_type::open(T(const_cast<U0&>(u0), u1), -1, -1);
    }
#if !NDNBOOST_WORKAROUND(NDNBOOST_MSVC, <= 1300) //---------------------------------//
    template<typename U0, typename U1>
    void open_impl(mpl::true_, U0& u0, const U1& u1)
    {
        base_type::open(T(u0, u1), -1, -1);
    }
#endif // !NDNBOOST_WORKAROUND(NDNBOOST_MSVC, <= 1300) //---------------------------//
    template<typename U0, typename U1, typename U2>
    void open_impl(mpl::false_, const U0& u0, const U1& u1, const U2& u2)
    {
        base_type::open(u0, u1, u2);
    }
    template<typename U0, typename U1, typename U2>
    void open_impl(mpl::true_, const U0& u0, const U1& u1, const U2& u2)
    {
        base_type::open(T(const_cast<U0&>(u0), u1, u2), -1, -1);
    }
#if !NDNBOOST_WORKAROUND(NDNBOOST_MSVC, <= 1300) //---------------------------------//
    template<typename U0, typename U1, typename U2>
    void open_impl(mpl::true_, U0& u0, const U1& u1, const U2& u2)
    {
        base_type::open(T(u0, u1, u2), -1, -1);
    }
#endif // !NDNBOOST_WORKAROUND(NDNBOOST_MSVC, <= 1300) //---------------------------//
    void check_open()
    {
        if (this->is_open()) 
            ndnboost::throw_exception(NDNBOOST_IOSTREAMS_FAILURE("already open"));
    }
};

} } // End namespaces iostreams, boost.

#endif // NDNBOOST_IOSTREAMS_DETAIL_BROKEN_OVERLOAD_RESOLUTION_STREAM_BUFFER_HPP_INCLUDED
