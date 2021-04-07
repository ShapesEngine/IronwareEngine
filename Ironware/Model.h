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

#include <assimp/scene.h>

class Mesh : public DrawableBase<Mesh>
{
public:
	Mesh( Graphics& gfx, std::vector<std::unique_ptr<Bindable>> bindablePtrs );
	void Draw( Graphics& gfx, DirectX::FXMMATRIX accumulatedTransform ) const noexcept( !IS_DEBUG );
	DirectX::XMMATRIX GetTransformXM() const noexcept override { return DirectX::XMLoadFloat4x4( &transform ); }

private:
	mutable DirectX::XMFLOAT4X4 transform = {};
};

class Node
{
	friend class Model;
public:
	Node( std::vector<Mesh*> meshPtrs, const DirectX::XMMATRIX& transform_in ) noexcept( !IS_DEBUG );
	void Draw( Graphics& gfx, DirectX::FXMMATRIX accumulatedTransform ) const noexcept( !IS_DEBUG );

private:
	void AddChild( std::unique_ptr<Node> pChild ) noexcept( !IS_DEBUG );

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
	void Draw( Graphics& gfx, DirectX::FXMMATRIX transform ) const noexcept( !IS_DEBUG ) { pRoot->Draw( gfx, transform ); }

private:
	std::unique_ptr<Mesh> ParseMesh( Graphics& gfx, const aiMesh& mesh ) noexcept( !IS_DEBUG );
	std::unique_ptr<Node> ParseNode( const aiNode& node ) noexcept( !IS_DEBUG );

private:
	std::vector<std::unique_ptr<Mesh>> meshPtrs;
	// we only keep pointer to the root
	// other nodes are used when the draw 
	// has been called
	std::unique_ptr<Node> pRoot;
};