// Copyright (c) 2020 by Chrono

#ifndef _HELLO_HPP
#define _HELLO_HPP

#include "cpplang.hpp"

extern "C" {
#include <luajit.h>
#include <lualib.h>
#include <lauxlib.h>
}

#include <LuaBridge/LuaBridge.h>

BEGIN_NAMESPACE(cpp_study)

class Config final
{
public:
    using vm_type           = std::shared_ptr<lua_State>;
    using value_type        = luabridge::LuaRef;

    using string_type       = std::string;
    using string_view_type  = const std::string&;
    using regex_type        = std::regex;
    using match_type        = std::smatch;
public:
    Config() noexcept
    {
        assert(m_vm);

        luaL_openlibs(m_vm.get());
    }

   ~Config() = default;
public:
    void load(string_view_type filename)
    {
        auto status = luaL_dofile(m_vm.get(), filename.c_str());

        if (status != 0) {
            throw std::runtime_error("failed to parse config");
        }
    }

    value_type get(string_view_type key)
    {
        if (!std::regex_match(key, m_what, m_conf_reg)) {
            throw std::runtime_error("config key error");
        }

        auto w1 = m_what[1].str();
        auto w2 = m_what[2].str();

        return luabridge::getGlobal(
                    m_vm.get(), w1.c_str())[w2];
    }
private:
    vm_type     m_vm {luaL_newstate(), lua_close};

    regex_type  m_conf_reg {R"(^(\w+)\.(\w+)$)"};
    match_type  m_what;
};

END_NAMESPACE(cpp_study)

#endif  //_HELLO_HPP
