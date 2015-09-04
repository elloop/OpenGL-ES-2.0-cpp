#pragma once

#include "GLES2/gl2.h"
#include "EGL/egl.h"
#include "elloop/inc.h"
#include "ShaderProgram.h"
#include "ShaderHelper.h"
#include "TextReader.h"
#include "Config.h"
#include "cell/CELLMath.hpp"

typedef GLuint ShaderId;
typedef GLuint ProgramId;
//typedef std::function<static LRESULT(HWND, UINT, WPARAM, LPARAM)> WindowCallback;
typedef std::function<LRESULT(HWND, UINT, WPARAM, LPARAM)> WindowCallback;

extern bool LOG_OPEN;

