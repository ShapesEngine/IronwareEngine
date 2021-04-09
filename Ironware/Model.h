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
#include "IronException.h"

#include <assimp/scene.h>

#include <optional>

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
	friend class ModelWindow;
public:
	Node( std::vector<Mesh*> meshPtrs, const std::string& name, uint32_t index, const DirectX::XMMATRIX& transform_in ) IFNOEXCEPT;
	void Draw( Graphics& gfx, DirectX::FXMMATRIX accumulatedTransform ) const IFNOEXCEPT;

private:
	void AddChild( std::unique_ptr<Node> pChild ) IFNOEXCEPT;
	void ShowTree( Node*& pSelectedNode ) const IFNOEXCEPT;
	void SetAppliedTransform( DirectX::FXMMATRIX transform ) noexcept;

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
	Model( Graphics& gfx, std::string filename );
	void Draw( Graphics& gfx ) const IFNOEXCEPT;
	void ShowWindow( const char* name = "Model" ) const IFNOEXCEPT;
	size_t GetNodeSize() const noexcept;
	~Model() noexcept;

private:
	std::unique_ptr<Mesh> ParseMesh( Graphics& gfx, const aiMesh& mesh ) IFNOEXCEPT;
	std::unique_ptr<Node> ParseNode( const aiNode& node ) IFNOEXCEPT;

private:
	std::vector<std::unique_ptr<Mesh>> meshPtrs;
	// we only keep pointer to the root
	// other nodes are used when the draw 
	// has been called
	std::unique_ptr<Node> pRoot;
	// pImpl
	std::unique_ptr<class ModelWindow> pModelWindow{ std::make_unique<ModelWindow>() };
	size_t nodeNum = 0;
};