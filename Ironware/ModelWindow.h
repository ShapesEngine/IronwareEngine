/*!
 * \file ModelWindow.h
 *
 * \author Yernar Aldabergenov
 * \date May 2021
 *
 *
 */
#pragma once

#include "Graphics.h"
#include "DynamicConstantBuffer.h"
#include "Node.h"

#include <unordered_map>
#include <optional>

 /*!
  * \class Model
  *
  * \brief pImpl idiom class that controls window of a model
  *
  * \author Yernar Aldabergenov
  * \date April 2021
  */
class ModelWindow
{
public:
	void ShowWindow( Graphics& gfx, const char* name, const Node& root ) IFNOEXCEPT;
	void ApplyParameters() IFNOEXCEPT;

	DirectX::XMMATRIX GetTransform() const IFNOEXCEPT;

	Node* GetSelectedNode() const noexcept { return pSelectedNode; }

	const Buffer& GetMaterial() const IFNOEXCEPT;

	bool TransformDirty() const IFNOEXCEPT { return pSelectedNode && transforms.at( pSelectedNode->GetID() ).transformParamsDirty; }

	void ResetTransformDirty() IFNOEXCEPT { transforms.at( pSelectedNode->GetID() ).transformParamsDirty = false; }

	bool MaterialDirty() const IFNOEXCEPT { return pSelectedNode && transforms.at( pSelectedNode->GetID() ).materialCbufDirty; }

	void ResetMaterialDirty() IFNOEXCEPT { transforms.at( pSelectedNode->GetID() ).materialCbufDirty = false; }

	bool IsDirty() const IFNOEXCEPT { return TransformDirty() || MaterialDirty(); }

private:
	struct TransformationParams
	{
		float roll = 0.f;
		float pitch = 0.f;
		float yaw = 0.f;
		float x = 0.f;
		float y = 0.f;
		float z = 0.f;
	};
	struct NodeData
	{
		TransformationParams tranformParams;
		bool transformParamsDirty;
		std::optional<Buffer> materialCbuf;
		bool materialCbufDirty;
	};
	std::unordered_map<uint32_t, NodeData> transforms;
	Node* pSelectedNode = nullptr;
};