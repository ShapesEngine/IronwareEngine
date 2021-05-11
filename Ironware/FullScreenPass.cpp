/*!
 * \file FullScreenPass.cpp
 *
 * \author Yernar Aldabergenov
 * \date May 2021
 *
 *
 */
#include "FullscreenPass.h"
#include "BindableCommon.h"

namespace dx = DirectX;

FullscreenPass::FullscreenPass( const std::string name, Graphics& gfx ) IFNOEXCEPT :
BindingPass( std::move( name ) )
{
	// setup fullscreen geometry
	VertexLayout lay;
	lay.Append( VertexLayout::ElementType::Position2D );
	VertexByteBuffer bufFull{ lay };
	bufFull.EmplaceBack( dx::XMFLOAT2{ -1,1 } );
	bufFull.EmplaceBack( dx::XMFLOAT2{ 1,1 } );
	bufFull.EmplaceBack( dx::XMFLOAT2{ -1,-1 } );
	bufFull.EmplaceBack( dx::XMFLOAT2{ 1,-1 } );
	AddBind( VertexBuffer::Resolve( gfx, L"$Full", std::move( bufFull ) ) );
	std::vector<uint16_t> indices = { 0,1,2,1,3,2 };
	AddBind( IndexBuffer::Resolve( gfx, L"$Full", std::move( indices ) ) );
	// setup other common fullscreen bindables
	auto vs = VertexShader::Resolve( gfx, L"Fullscreen_VS.cso" );
	AddBind( InputLayout::Resolve( gfx, lay, *vs ) );
	AddBind( std::move( vs ) );
	AddBind( PrimitiveTopology::Resolve( gfx ) );
	AddBind( RasterizerState::Resolve( gfx, false ) );
}

void FullscreenPass::Execute( Graphics& gfx ) const IFNOEXCEPT
{
	BindAll( gfx );
	gfx.DrawIndexed( 6u );
}