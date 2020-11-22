/*!
 * \file GraphicsExceptionMacros.h
 * \date 2020/11/23 4:14
 *
 * \author Yernar Aldabergenov
 * Contact: yernar.aa@gmail.com
 *
 * \brief Macros that can be useful when there is need to use boilerplate code for testing(exception throwing) in Graphics
 *
 * TODO:
 *
 * \note HRESULT hr should exist in the local space for the particular macros to work;
 * * Some macros use info manager, but info manager is only available at debug mode!
*/
#pragma once

// =======================================================================
// graphics exception checking/throwing macros (some with dxgi infos)
// -----------------------------------------------------------------------
#define GFX_EXCEPT_NOINFO(hr) Graphics::HrException( __LINE__, WFILE, (hr) )
#define GFX_THROW_NOINFO(hrcall) if( FAILED( hr = (hrcall) ) ) throw Graphics::HrException( __LINE__, WFILE, hr )

#ifndef NDEBUG
#define GFX_THROW_INFO(hrcall) infoManager.Set(); if( FAILED( hr = (hrcall) ) ) throw GFX_GET_EXCEPT_ERROR_TYPE(hr)
#define GFX_GET_EXCEPT_ERROR_TYPE(hr) (hr == DXGI_ERROR_DEVICE_REMOVED ? GFX_DEVICE_REMOVED_EXCEPT(hr) : GFX_EXCEPT(hr))
#define GFX_DEVICE_REMOVED_EXCEPT(hr) Graphics::DeviceRemovedException( __LINE__, WFILE, (hr), infoManager.GetMessages() )
#define GFX_EXCEPT(hr) Graphics::HrException( __LINE__, WFILE, (hr), infoManager.GetMessages() )
#define GFX_THROW_INFO_ONLY(call) infoManager.Set(); (call); GFX_THROW_UNHANDLED_EXCEPTION
#define GFX_THROW_UNHANDLED_EXCEPTION {auto v = infoManager.GetMessages(); if(!v.empty()) {throw Graphics::InfoException( __LINE__,WFILE,v);}}
#else
#define GFX_EXCEPT(hr) Graphics::HrException( __LINE__, WFILE, (hr) )
#define GFX_THROW_INFO(hrcall) GFX_THROW_NOINFO(hrcall)
#define GFX_DEVICE_REMOVED_EXCEPT(hr) Graphics::DeviceRemovedException( __LINE__, WFILE, (hr) )
#define GFX_THROW_INFO_ONLY(call) (call)
#endif
// =======================================================================

// macro for importing infomanager into local scope
// this.GetInfoManager() must exist
#ifdef NDEBUG
#define INFOMAN() HRESULT hr
#else
#define INFOMAN() HRESULT hr; DxgiInfoManager& infoManager = GetInfoManager()
#endif