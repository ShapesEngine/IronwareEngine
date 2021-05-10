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

class Node
{
	friend class Model;
	friend class ModelWindow;
public:
	Node( std::vector<Mesh*> meshPtrs, const std::string& name, uint32_t index, const DirectX::XMMATRIX& transform_in ) IFNOEXCEPT;
	void Submit( DirectX::FXMMATRIX accumulatedTransform ) const IFNOEXCEPT;
	void Accept( class ModelProbe& probe );
	void Accept( class TechniqueProbe& probe );

	bool HasChildren() const noexcept { return !childPtrs.empty(); }
	const std::string& GetName() const noexcept { return name; }
	uint32_t GetID() const noexcept { return index; }
	void SetAppliedTransform( DirectX::FXMMATRIX transform ) noexcept { DirectX::XMStoreFloat4x4( &appliedTransform, transform ); }
	const DirectX::XMFLOAT4X4& GetAppliedTransform() const noexcept { return appliedTransform; }

private:
	void AddChild( std::unique_ptr<Node> pChild ) IFNOEXCEPT;

private:
	std::string name;
	uint32_t index;
	std::vector<Mesh*> meshPtrs;
	std::vector<std::unique_ptr<Node>> childPtrs;
	DirectX::XMFLOAT4X4 parentTransform;
	DirectX::XMFLOAT4X4 appliedTransform;
};