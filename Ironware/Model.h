/*!
 * \file Model.h
 *
 * \author Yernar Aldabergenov
 * \date March 2021
 *
 *
 */
#pragma once

#include "DrawableBase.h"
#include "CommonMacros.h"

#include <assimp/scene.h>

class Mesh : public DrawableBase<Mesh>
{
public:
	Mesh( Graphics& gfx, std::vector<std::unique_ptr<Bindable>> bindablePtrs );
	void Draw( Graphics& gfx, DirectX::FXMMATRIX accumulatedTransform ) const IFNOEXCEPT;
	DirectX::XMMATRIX GetTransformXM() const noexcept override { return DirectX::XMLoadFloat4x4( &transform ); }

private:
	mutable DirectX::XMFLOAT4X4 transform = {};
};

class Node
{
	friend class Model;
public:
	Node( std::vector<Mesh*> meshPtrs, const DirectX::XMMATRIX& transform_in ) IFNOEXCEPT;
	void Draw( Graphics& gfx, DirectX::FXMMATRIX accumulatedTransform ) const IFNOEXCEPT;

private:
	void AddChild( std::unique_ptr<Node> pChild ) IFNOEXCEPT;

private:
	std::vector<Mesh*> meshPtrs;
	std::vector<std::unique_ptr<Node>> childPtrs;
	DirectX::XMFLOAT4X4 transform;
};

/**
 * @brief Responsible class for drawing a mesh from specified file
*/
class Model
{
public:
	Model( Graphics& gfx, std::string filename );
	void Draw( Graphics& gfx, DirectX::FXMMATRIX transform ) const IFNOEXCEPT { pRoot->Draw( gfx, transform ); }

private:
	std::unique_ptr<Mesh> ParseMesh( Graphics& gfx, const aiMesh& mesh ) IFNOEXCEPT;
	std::unique_ptr<Node> ParseNode( const aiNode& node ) IFNOEXCEPT;

private:
	std::vector<std::unique_ptr<Mesh>> meshPtrs;
	// we only keep pointer to the root
	// other nodes are used when the draw 
	// has been called
	std::unique_ptr<Node> pRoot;
};