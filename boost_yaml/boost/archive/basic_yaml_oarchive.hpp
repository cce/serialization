#ifndef BOOST_ARCHIVE_BASIC_YAML_OARCHIVE_HPP
#define BOOST_ARCHIVE_BASIC_YAML_OARCHIVE_HPP

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
// basic_yaml_oarchive.hpp

// (C) Copyright 2008 Esteve Fernandez <esteve.fernandez@gmail.com> 
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org for updates, documentation, and revision history.

#include <boost/config.hpp>

#include <boost/archive/detail/common_oarchive.hpp>

#include <boost/serialization/cnv.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/serialization/tracking.hpp>
#include <boost/serialization/string.hpp>

#include <boost/archive/detail/abi_prefix.hpp> // must be the last header

#include <map>
#include <sstream>
#include <stack>

namespace boost { 
namespace archive {
        
//////////////////////////////////////////////////////////////////////
// class basic_yaml_oarchive - write serialized objects to a yaml output stream
template<class Archive>
class basic_yaml_oarchive : 
    public detail::common_oarchive<Archive>
{
protected:
#if BOOST_WORKAROUND(BOOST_MSVC, <= 1300)
public:
#elif defined(BOOST_MSVC)
    // for some inexplicable reason insertion of "class" generates compile erro
    // on msvc 7.1
    friend detail::interface_oarchive<Archive>;
#else
    friend class detail::interface_oarchive<Archive>;
#endif
    // special stuff for yaml output
    unsigned int depth;
    bool indent_next;
    bool pending_preamble;
    std::map<std::string, std::string> preamble;
    std::stack<std::string> class_stack;
    std::stack<std::string> hierarchy_stack;
    BOOST_ARCHIVE_OR_WARCHIVE_DECL(void)
    indent();
    BOOST_ARCHIVE_OR_WARCHIVE_DECL(void)
    init();
    BOOST_ARCHIVE_OR_WARCHIVE_DECL(void)
    write_attribute(
        const char *attribute_name, 
        int t,
        const char *conjunction = "=\""
    );
    BOOST_ARCHIVE_OR_WARCHIVE_DECL(void)
    write_attribute(
        const char *attribute_name, 
        const char *key
    );
    // helpers used below
    BOOST_ARCHIVE_OR_WARCHIVE_DECL(void)
    save_start(const char *name);
    BOOST_ARCHIVE_OR_WARCHIVE_DECL(void)
    save_start(const char *cname, const char *name);
    BOOST_ARCHIVE_OR_WARCHIVE_DECL(void)
    save_start_type(const char *cname, const char *name);

    BOOST_ARCHIVE_OR_WARCHIVE_DECL(void)
    save_end(const char *name);
    BOOST_ARCHIVE_OR_WARCHIVE_DECL(void)
    save_end(const char *cname, const char *name);
    BOOST_ARCHIVE_OR_WARCHIVE_DECL(void)
    save_end_type(const char *cname, const char *name);

    BOOST_ARCHIVE_OR_WARCHIVE_DECL(void)
    end_preamble();

    // Anything not an attribute and not a name-value pair is an
    // error and should be trapped here.
    template<class T>
    void save_override(T & t, BOOST_PFTO int)
    {
        // If your program fails to compile here, its most likely due to
        // not specifying an nvp wrapper around the variable to
        // be serialized.
        BOOST_STATIC_ASSERT(0 == sizeof(T));
    }

   // special treatment for name-value pairs.
    typedef detail::common_oarchive<Archive> detail_common_oarchive;
    template<class T>
    void save_override(
        #ifndef BOOST_NO_FUNCTION_TEMPLATE_ORDERING
        const
        #endif
        ::boost::serialization::nvp<T> & t, 
        int
    ){
        save_start(t.name());
        this->detail_common_oarchive::save_override(t.const_value(), 0);
        save_end(t.name());
    }

    // special treatment for name-value pairs.
    template<class T>
    void save_override(
        #ifndef BOOST_NO_FUNCTION_TEMPLATE_ORDERING
        const
        #endif
        ::boost::serialization::cnv<T> & t, 
        int
    ){
        save_start(t.cname(), t.name());
        this->detail_common_oarchive::save_override(t.const_value(), 0);
        save_end(t.cname(), t.name());
    }

    // special treatment for name-value pairs.
    template<class T>
    void save_override(
        #ifndef BOOST_NO_FUNCTION_TEMPLATE_ORDERING
        const
        #endif
        ::boost::serialization::cnv_type<T> & t, 
        int
    ){
        save_start_type(t.cname(), t.name());
        this->detail_common_oarchive::save_override(t.const_value(), 0);
        save_end_type(t.cname(), t.name());
    }

    // specific overrides for attributes - not name value pairs so we
    // want to trap them before the above "fall through"
    BOOST_ARCHIVE_OR_WARCHIVE_DECL(void)
    save_override(const object_id_type & t, int);
    BOOST_ARCHIVE_OR_WARCHIVE_DECL(void)
    save_override(const object_reference_type & t, int);
    BOOST_ARCHIVE_OR_WARCHIVE_DECL(void)
    save_override(const version_type & t, int);
    BOOST_ARCHIVE_OR_WARCHIVE_DECL(void)
    save_override(const class_id_type & t, int);
    BOOST_ARCHIVE_OR_WARCHIVE_DECL(void)
    save_override(const class_id_optional_type & t, int);
    BOOST_ARCHIVE_OR_WARCHIVE_DECL(void)
    save_override(const class_id_reference_type & t, int);
    BOOST_ARCHIVE_OR_WARCHIVE_DECL(void)
    save_override(const class_name_type & t, int);
    BOOST_ARCHIVE_OR_WARCHIVE_DECL(void)
    save_override(const tracking_type & t, int);

    BOOST_ARCHIVE_OR_WARCHIVE_DECL(BOOST_PP_EMPTY()) 
    basic_yaml_oarchive(unsigned int flags);
    BOOST_ARCHIVE_OR_WARCHIVE_DECL(BOOST_PP_EMPTY()) 
    ~basic_yaml_oarchive();
};

} // namespace archive
} // namespace boost

#include <boost/archive/detail/abi_suffix.hpp> // pops abi_suffix.hpp pragmas

#endif // BOOST_ARCHIVE_BASIC_YAML_OARCHIVE_HPP
