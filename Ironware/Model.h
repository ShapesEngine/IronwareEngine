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

class Material;

class Mesh : public Drawable
{
public:
	Mesh( Graphics& gfx,const Material& mat,const aiMesh& mesh ) IFNOEXCEPT;
	void Submit( FrameExecutor& frame, DirectX::FXMMATRIX accumulatedTransform ) const IFNOEXCEPT;
	DirectX::XMMATRIX GetTransformXM() const noexcept override { return DirectX::XMLoadFloat4x4( &transform ); }

private:
	mutable DirectX::XMFLOAT4X4 transform = {};
};

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
	const Buffer* GetMaterialConstants() const IFNOEXCEPT;
	void SetMaterialConstants( const Buffer& ) IFNOEXCEPT;
	
	template<typename M>
	bool ShowControlMaterial( Graphics& gfx, M& pMC );

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

/**
 * @brief Responsible class for drawing a mesh from specified file
*/
class Model
{
public:
	/**
	* @brief Class that manages error checking for the model
	*/
	class Exception : public IronException
	{
	public:
		Exception( int line, const wchar_t* filename, const std::string& note ) noexcept :
			IronException( line, filename ),
			note( note )
		{}
		const wchar_t* GetType() const noexcept override { return type.c_str(); }
		const std::string& GetNote() const noexcept { return note; }
		const char* what() const noexcept override;

	private:
		std::string note;
		std::wstring type = L"Ironware Model Exception";
	};

public:
	Model( Graphics& gfx, std::wstring path, float scale = 1.f, DirectX::XMFLOAT3 startingPos = { 0.f, 0.f, 0.f } );
	void Submit( FrameExecutor& frame ) const IFNOEXCEPT;
	void ShowWindow( Graphics& gfx, const char* name = "Model" ) const IFNOEXCEPT;
	size_t GetNodeSize() const noexcept;
	~Model() noexcept;

private:
	std::unique_ptr<Mesh> ParseMesh( Graphics& gfx, const aiMesh& mesh, const aiMaterial* const* pMaterials ) IFNOEXCEPT;
	std::unique_ptr<Node> ParseNode( const aiNode& node ) IFNOEXCEPT;

private:
	std::vector<std::unique_ptr<Mesh>> meshPtrs;
	// we only keep pointer to the root
	// other nodes are used when the draw 
	// has been called
	std::unique_ptr<Node> pRoot;
	std::wstring path;
	float scale;
	// pImpl
	std::unique_ptr<class ModelWindow> pModelWindow{ std::make_unique<ModelWindow>() };
	size_t nodeNum = 0;
};