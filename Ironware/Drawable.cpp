/*!
 * \file Drawable.cpp
 *
 * \author Yernar Aldabergenov
 * \date September 2020
 *
 *
 */
#include "Drawable.h"
#include "GraphicsExceptionMacros.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "PrimitiveTopology.h"

#include <cassert>

void Drawable::AddTechnique( RenderTechnique tech_in ) noexcept
{
	tech_in.InitializeParentReferences( *this );
	techniques.push_back( std::move( tech_in ) );
}

void Drawable::Submit( FrameExecutor & frame ) const noexcept
{
	for( const auto& tech : techniques )
	{
		tech.Submit( frame, *this );
	}
}

void Drawable::Bind( Graphics & gfx ) const noexcept
{
	pTopology->Bind( gfx );
	pIndices->Bind( gfx );
	pVertices->Bind( gfx );
}

UINT Drawable::GetIndexCount() const IFNOEXCEPT
{
	return pIndices->GetCount();
}