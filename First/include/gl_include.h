#pragma once

#include "GLES2/gl2.h"
#include "EGL/egl.h"
#include "elloop/inc.h"
#include "scenes/ELShaderProgram.h"
#include "util/ELShaderHelper.h"
#include "util/ELTextReader.h"
#include "math/CELLMath.hpp"

typedef GLuint ShaderId;
typedef GLuint ProgramId;
//typedef std::function<static LRESULT(HWND, UINT, WPARAM, LPARAM)> WindowCallback;
typedef std::function<LRESULT(HWND, UINT, WPARAM, LPARAM)> WindowCallback;

extern bool LOG_OPEN;

