/*!
 * \class DxgiInfoManager
 *
 * \ingroup DEV
 *
 * \brief A helper class that is used for getting DXGI error information
 *
 * TODO:
 *
 * \note Call Set every time you want to capture an error. Call => DirectXFunc => GetMessages
 *
 * \author Yernar Aldabergenov
 *
 * \version 1.0
 *
 * \date November 2020
 *
 * Contact: yernar.aa@gmail.com
 *
 */
#include "DxgiInfoManager.h"
#include "Window.h"
#include "Graphics.h"
#include "IronUtils.h"

#include <dxgidebug.h>
#include <memory>

#pragma comment(lib, "dxguid.lib")

#define GFX_THROW_NOINFO(hrcall) if( FAILED( hr = (hrcall) ) ) throw Graphics::HrException( __LINE__, WFILE, hr )

DxgiInfoManager::DxgiInfoManager()
{
	// define function signature of DXGIGetDebugInterface
	typedef HRESULT (WINAPI* DXGIGetDebugInterface)(REFIID,void **);

	// load the dll that contains the function DXGIGetDebugInterface
	const auto hModDxgiDebug = LoadLibraryEx( L"dxgidebug.dll",nullptr,LOAD_LIBRARY_SEARCH_SYSTEM32 );
	if( hModDxgiDebug == nullptr )
	{
		throw IRWND_LAST_EXCEPT();
	}

	// get address of DXGIGetDebugInterface in dll
	const auto DxgiGetDebugInterface = reinterpret_cast<DXGIGetDebugInterface>(
		reinterpret_cast<void*>( GetProcAddress( hModDxgiDebug, "DXGIGetDebugInterface" ) )
		);
	if( DxgiGetDebugInterface == nullptr )
	{
		throw IRWND_LAST_EXCEPT();
	}

	HRESULT hr;
	GFX_THROW_NOINFO( DxgiGetDebugInterface( __uuidof(IDXGIInfoQueue),reinterpret_cast<void**>(&pDxgiInfoQueue) ) );
}

DxgiInfoManager::~DxgiInfoManager()
{
	if( pDxgiInfoQueue != nullptr )
	{
		pDxgiInfoQueue->Release();
	}
}

void DxgiInfoManager::Set() noexcept
{
	// set the index (next) so that the next all to GetMessages()
	// will only get errors generated after this call
	next = pDxgiInfoQueue->GetNumStoredMessages( DXGI_DEBUG_ALL );
}

std::vector<std::wstring> DxgiInfoManager::GetMessages() const
{
	std::vector<std::wstring> messages;
	const auto end = pDxgiInfoQueue->GetNumStoredMessages( DXGI_DEBUG_ALL );
	for( auto i = next; i < end; i++ )
	{
		HRESULT hr;
		SIZE_T messageLength;
		// get the size of message i in bytes
		GFX_THROW_NOINFO( pDxgiInfoQueue->GetMessage( DXGI_DEBUG_ALL, i, nullptr, &messageLength ) );
		// allocate memory for message
		auto bytes = std::make_unique<byte[]>( messageLength );
		auto pMessage = reinterpret_cast<DXGI_INFO_QUEUE_MESSAGE*>( bytes.get() );
		// get the message and push its description into the vector
		GFX_THROW_NOINFO( pDxgiInfoQueue->GetMessage( DXGI_DEBUG_ALL, i, pMessage, &messageLength ) );
		messages.emplace_back( ToWide( pMessage->pDescription ) );
	}
	return messages;
}
