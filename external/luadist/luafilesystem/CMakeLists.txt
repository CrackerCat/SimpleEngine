# Copyright (C) 2007-2012 LuaDist.
# Created by Peter Drahoš
# Redistribution and use of this file is allowed according to the terms of the MIT license.
# For details see the COPYRIGHT file distributed with LuaDist.
# Please note that the package source code is licensed under its own license.

project ( luafilesystem C )
cmake_minimum_required ( VERSION 2.8 )
include ( cmake/dist.cmake )
include ( lua )

install_lua_module ( lfs src/lfs.c vc6/lfs.def )
install_doc ( doc/ )
install_test ( tests/ )
install_data ( README )

add_lua_test ( tests/test.lua )
