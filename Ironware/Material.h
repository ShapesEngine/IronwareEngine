/*!
 * \file Material.h
 *
 * \author Yernar Aldabergenov
 * \date May 2021
 *
 * 
 */
#pragma once

#include "Vertex.h"
#include "RenderTechnique.h"
#include "DynamicConstantBuffer.h"
#include "ConstantBuffersEx.h"
#define IR_INCLUDE_TEXTURE
#include "BindableCommon.h"

#include <vector>
#include <filesystem>

class Material
{
public:
	Material( Graphics& gfx, const aiMaterial& material, const std::filesystem::path& path ) IFNOEXCEPT;
	VertexByteBuffer ExtractVertices( const aiMesh& mesh ) const noexcept;
	std::vector<uint16_t> ExtractIndices( const aiMesh& mesh ) const noexcept
	{
		std::vector<uint16_t> indices;
		indices.reserve( mesh.mNumFaces * 3 );
		for( unsigned int i = 0; i < mesh.mNumFaces; i++ )
		{
			const auto& face = mesh.mFaces[i];
			assert( face.mNumIndices == 3 );
			indices.push_back( face.mIndices[0] );
			indices.push_back( face.mIndices[1] );
			indices.push_back( face.mIndices[2] );
		}
		return indices;
	}
	std::shared_ptr<VertexBuffer> MakeVertexBindable( Graphics& gfx, const aiMesh& mesh ) const IFNOEXCEPT
	{
		return VertexBuffer::Resolve( gfx, MakeMeshTag( mesh ), ExtractVertices( mesh ) );
	}
	std::shared_ptr<IndexBuffer> MakeIndexBindable( Graphics& gfx, const aiMesh& mesh ) const IFNOEXCEPT
	{
		return IndexBuffer::Resolve( gfx, MakeMeshTag( mesh ), ExtractIndices( mesh ) );
	}
	std::vector<RenderTechnique> GetTechniques() const noexcept;
private:
	std::wstring MakeMeshTag( const aiMesh& mesh ) const noexcept { return modelPath + L"$" + to_wide( mesh.mName.C_Str() ); }
private:
	VertexLayout vtxLayout;
	std::vector<RenderTechnique> techniques;
	std::wstring modelPath;
	std::wstring name;
};