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
#include <assimp/scene.h>
#include "Material.h"

#include <cassert>

Drawable::Drawable( Graphics& gfx, const Material& mat, const aiMesh& mesh ) noexcept
{
	pVertices = mat.MakeVertexBindable( gfx, mesh );
	pIndices = mat.MakeIndexBindable( gfx, mesh );
	pTopology = PrimitiveTopology::Resolve( gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );

	for( auto& t : mat.GetTechniques() )
	{
		AddTechnique( std::move( t ) );
	}
}

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

void Drawable::Accept( TechniqueProbe & probe )
{
	for( auto& t : techniques )
	{
		t.Accept( probe );
	}
}

UINT Drawable::GetIndexCount() const IFNOEXCEPT
{
	return pIndices->GetCount();
}