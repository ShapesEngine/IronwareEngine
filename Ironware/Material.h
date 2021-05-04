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
	std::vector<uint16_t> ExtractIndices( const aiMesh& mesh ) const noexcept;
	std::shared_ptr<VertexBuffer> MakeVertexBindable( Graphics& gfx, const aiMesh& mesh ) const IFNOEXCEPT;
	std::shared_ptr<IndexBuffer> MakeIndexBindable( Graphics& gfx, const aiMesh& mesh ) const IFNOEXCEPT;
	std::vector<RenderTechnique> GetTechniques() const noexcept;

private:
	std::wstring MakeMeshTag( const aiMesh& mesh ) const noexcept { return modelPath + L"$" + to_wide( mesh.mName.C_Str() ); }

private:
	VertexLayout vtxLayout;
	std::vector<RenderTechnique> techniques;
	std::wstring modelPath;
	std::wstring name;
};