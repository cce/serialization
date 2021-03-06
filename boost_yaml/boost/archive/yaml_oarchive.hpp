#ifndef BOOST_ARCHIVE_YAML_OARCHIVE_HPP
#define BOOST_ARCHIVE_YAML_OARCHIVE_HPP

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
// yaml_oarchive.hpp

// (C) Copyright 2008 Esteve Fernandez <esteve.fernandez@gmail.com> 
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org for updates, documentation, and revision history.

#include <ostream>

#include <cstddef> // size_t
#include <boost/config.hpp>
#if defined(BOOST_NO_STDC_NAMESPACE)
namespace std{ 
    using ::size_t; 
} // namespace std
#endif

#include <boost/archive/detail/auto_link_archive.hpp>
#include <boost/archive/basic_text_oprimitive.hpp>
#include <boost/archive/basic_yaml_oarchive.hpp>

#include <boost/archive/detail/abi_prefix.hpp> // must be the last header

namespace boost {
namespace archive {

template<class Archive>
class yaml_oarchive_impl : 
    public basic_text_oprimitive<std::ostream>,
    public basic_yaml_oarchive<Archive>
{
#ifdef BOOST_NO_MEMBER_TEMPLATE_FRIENDS
public:
#else
    friend class detail::interface_oarchive<Archive>;
    friend class basic_yaml_oarchive<Archive>;
    friend class save_access;
protected:
#endif
    void end_preamble(){
        basic_yaml_oarchive<Archive>::end_preamble();
    }
    template<class T>
    void save(const T & t){
        basic_text_oprimitive<std::ostream>::save(t);
    }
    BOOST_ARCHIVE_DECL(void) 
    save(const char * t);
    #ifndef BOOST_NO_INTRINSIC_WCHAR_T
    BOOST_ARCHIVE_DECL(void)
    save(const wchar_t * t);
    #endif
    BOOST_ARCHIVE_DECL(void)
    save(const std::string &s);
    #ifndef BOOST_NO_STD_WSTRING
    BOOST_ARCHIVE_DECL(void)
    save(const std::wstring &ws);
    #endif
    BOOST_ARCHIVE_DECL(BOOST_PP_EMPTY()) 
    yaml_oarchive_impl(std::ostream & os, unsigned int flags);
    ~yaml_oarchive_impl(){}
public:
    void save_binary(const void *address, std::size_t count){
        this->end_preamble();
        #if ! defined(__MWERKS__)
        this->basic_text_oprimitive<std::ostream>::save_binary(
        #else
        this->basic_text_oprimitive::save_binary(
        #endif
            address, 
            count
        );
        this->indent_next = true;
    }
};

// we use the following because we can't use
// typedef yaml_oarchive_impl<yaml_oarchive_impl<...> > yaml_oarchive;

// do not derive from this class.  If you want to extend this functionality
// via inhertance, derived from yaml_oarchive_impl instead.  This will
// preserve correct static polymorphism.
class yaml_oarchive : 
    public yaml_oarchive_impl<yaml_oarchive>
{
public:
    yaml_oarchive(std::ostream & os, unsigned int flags = 0) :
        yaml_oarchive_impl<yaml_oarchive>(os, flags)
    {}
    ~yaml_oarchive(){}
};

} // namespace archive
} // namespace boost

// required by smart_cast for compilers not implementing 
// partial template specialization
BOOST_BROKEN_COMPILER_TYPE_TRAITS_SPECIALIZATION(boost::archive::yaml_oarchive)

#include <boost/archive/detail/abi_suffix.hpp> // pops abi_suffix.hpp pragmas

#define BOOST_ARCHIVE_CUSTOM_OARCHIVE_TYPES boost::archive::yaml_oarchive

#endif // BOOST_ARCHIVE_YAML_OARCHIVE_HPP
