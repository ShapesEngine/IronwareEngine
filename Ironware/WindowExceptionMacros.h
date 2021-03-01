/*!
 * \file WindowExceptionMacros.h
 * \date 2020/09/23 4:19
 *
 * \author Yernar Aldabergenov
 * Contact: yernar.aa@gmail.com
 *
 * \brief Macros that can be useful when there is need to use boilerplate code for testing(exception throwing) in Graphics
 *
 * TODO:
 *
 * \note Make sure that IronWin is included when this header is used, as IronWin contains WIDE form of __FILE__
*/
#pragma once

// =======================================================================
// window error exception helper macros
// -----------------------------------------------------------------------
#define IRWND_EXCEPT( hr ) Window::HrException( __LINE__, WFILE, hr )
#define IRWND_LAST_EXCEPT() Window::HrException( __LINE__, WFILE, GetLastError() )
#define IRWND_NOGFX_EXCEPT() Window::NoGfxException( __LINE__, WFILE )
// =======================================================================