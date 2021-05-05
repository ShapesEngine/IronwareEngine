/*!
 * \file Model.h
 *
 * \author Yernar Aldabergenov
 * \date March 2021
 *
 *
 */
#pragma once

#include "Drawable.h"
#include "CommonMacros.h"
#include "IronException.h"
#include "DynamicConstantBuffer.h"

#include <assimp/scene.h>
#include <imgui/imgui.h>
#include <string>

#include <optional>
#include <type_traits>

class Node;
class Mesh;

/**
 * @brief Responsible class for drawing a mesh from specified file
*/
class Model
{
public:
	Model( Graphics& gfx, std::wstring path, float scale = 1.f, DirectX::XMFLOAT3 startingPos = { 0.f, 0.f, 0.f } );
	void Submit( FrameExecutor& frame ) const IFNOEXCEPT;

	void Accept( class ModelProbe& probe );
	size_t GetNodeSize() const noexcept { return nodeNum; }
	~Model() noexcept;

private:
	static std::unique_ptr<Mesh> ParseMesh( Graphics& gfx, const aiMesh& mesh, const aiMaterial* const* pMaterials ) IFNOEXCEPT;
	std::unique_ptr<Node> ParseNode( const aiNode& node, float scale ) IFNOEXCEPT;

private:
	std::vector<std::unique_ptr<Mesh>> meshPtrs;
	// we only keep pointer to the root
	// other nodes are used when the draw 
	// has been called
	std::unique_ptr<Node> pRoot;
	std::wstring path;
	float scale;
	// pImpl
	/*std::unique_ptr<class ModelWindow> pModelWindow{ std::make_unique<ModelWindow>() };*/
	size_t nodeNum;
};