#include "LuaPainter.h"

using luaUi::LuaPainter;

LuaPainter::LuaPainter() {
    lua = luaL_newstate();
    luaL_openlibs(lua);
}

LuaPainter::~LuaPainter() {
    lua_close(lua);
}

void LuaPainter::setup() {

}

void LuaPainter::paint() {
    lua_getglobal(lua, "require");
    lua_pushliteral(lua, "main");
    lua_call(lua, 1, 1);

    if (lua_getfield(lua, -1, "paint") != LUA_TFUNCTION) {
        lua_pop(lua, 2);
        return;
    }

    lua_call(lua, 0, 0);
    lua_pop(lua, 1);
}
