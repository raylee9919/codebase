// Copyright (c) 2025 Seong Woo Lee. All rights reserved.
#ifndef LSW_RENDER_H_
#define LSW_RENDER_H

#ifdef OS_WINDOWS
#  ifndef BUILD_CLI
#    include "render/backend/d3d11.h"
#    include "render/backend/d3d11.cpp"
#  endif
#else
#  error Define OS: OS_WINDOWS|OS_LINUX|OS_MAC
#endif



#endif //LSW_RENDER_H_
