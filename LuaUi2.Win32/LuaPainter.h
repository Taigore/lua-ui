#pragma once

#include "WindowPainter.h"

#include <lua.hpp>

namespace luaUi {
    class LuaPainter : public WindowPainter {
    public:
        LuaPainter();
        virtual ~LuaPainter() override;

        void setup();
        virtual void paint() override;

    private:
        lua_State* lua;
    };
}
