/*!
 * \file Node.h
 *
 * \author Yernar Aldabergenov
 * \date May 2021
 *
 *
 */
#pragma once

#include "Graphics.h"
#include "DynamicConstantBuffer.h"

class Mesh;
class FrameExecutor;

class Node
{
	friend class Model;
	friend class ModelWindow;
public:
	Node( std::vector<Mesh*> meshPtrs, const std::string& name, uint32_t index, const DirectX::XMMATRIX& transform_in ) IFNOEXCEPT;
	void Submit( FrameExecutor& frame, DirectX::FXMMATRIX accumulatedTransform ) const IFNOEXCEPT;

private:
	void AddChild( std::unique_ptr<Node> pChild ) IFNOEXCEPT;
	void ShowTree( Node*& pSelectedNode ) const IFNOEXCEPT;

	void SetAppliedTransform( DirectX::FXMMATRIX transform ) noexcept { DirectX::XMStoreFloat4x4( &appliedTransform, transform ); }
	const DirectX::XMFLOAT4X4& GetAppliedTransform() const noexcept { return appliedTransform; }
	uint32_t GetID() const noexcept { return index; }

private:
	std::string name;
	uint32_t index;
	std::vector<Mesh*> meshPtrs;
	std::vector<std::unique_ptr<Node>> childPtrs;
	DirectX::XMFLOAT4X4 parentTransform;
	DirectX::XMFLOAT4X4 appliedTransform;
};