// Copyright (c) 2025 Seong Woo Lee. All rights reserved.
#ifndef LSW_RENDER_H_
#define LSW_RENDER_H

#if OS_WINDOWS==1 && BUILD_CLI==0
#  include "render/backend/d3d11.h"
#  include "render/backend/d3d11.cpp"
#else
#  error Define OS: OS_WINDOWS|OS_LINUX|OS_MAC
#endif



#endif //LSW_RENDER_H_
