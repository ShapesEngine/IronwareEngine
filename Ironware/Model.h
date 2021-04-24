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

#include <assimp/scene.h>
#include <imgui/imgui.h>

#include <optional>
#include <type_traits>

class Mesh : public Drawable
{
public:
	Mesh( Graphics& gfx, std::vector<std::shared_ptr<Bindable>> bindablePtrs );
	void Draw( Graphics& gfx, DirectX::FXMMATRIX accumulatedTransform ) const IFNOEXCEPT;
	DirectX::XMMATRIX GetTransformXM() const noexcept override { return DirectX::XMLoadFloat4x4( &transform ); }

private:
	mutable DirectX::XMFLOAT4X4 transform = {};
};

class Node
{
	friend class Model;
	friend class ModelWindow;

private:
	struct PSMaterialConstantFull
	{
		BOOL normalMapEnabled = TRUE;
		BOOL specularMapEnabled = TRUE;
		BOOL hasGlossMap = FALSE;
		float specularPower = 3.3f;
		DirectX::XMFLOAT3 specularColor = { 0.75f, 0.75f, 0.75f };
		float specularMapWeight = 0.65f;
	};

	struct PSMaterialConstantNoTex
	{
		DirectX::XMFLOAT4 materialColor = { 0.447970f, 0.327254f, 0.176283f, 1.f };
		DirectX::XMFLOAT4 specularColor = { 0.65f, 0.65f, 0.65f, 1.f };
		float specularPower = 120.f;
		float padding[3];
	};

public:
	Node( std::vector<Mesh*> meshPtrs, const std::string& name, uint32_t index, const DirectX::XMMATRIX& transform_in ) IFNOEXCEPT;
	void Draw( Graphics& gfx, DirectX::FXMMATRIX accumulatedTransform ) const IFNOEXCEPT;

private:
	void AddChild( std::unique_ptr<Node> pChild ) IFNOEXCEPT;
	void ShowTree( Node*& pSelectedNode ) const IFNOEXCEPT;
	void SetAppliedTransform( DirectX::FXMMATRIX transform ) noexcept;
	template<typename M>
	bool ShowControlMaterial( Graphics& gfx, M& pMC );

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
	void Draw( Graphics& gfx ) const IFNOEXCEPT;
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

template<typename M>
bool Node::ShowControlMaterial( Graphics& gfx, M& pMC )
{
	if( meshPtrs.empty() )
	{
		return false;
	}

	if constexpr( std::is_same<PSMaterialConstantFull, M>::value )
	{
		if( auto pcb = meshPtrs.front()->QueryBindable<PixelConstantBuffer<PSMaterialConstantFull>>() )
		{
			ImGui::Text( "Material" );

			bool normalMapEnabled = (bool)pMC.normalMapEnabled;
			ImGui::Checkbox( "Norm Map", &normalMapEnabled );
			pMC.normalMapEnabled = normalMapEnabled ? TRUE : FALSE;

			bool specularMapEnabled = (bool)pMC.specularMapEnabled;
			ImGui::Checkbox( "Spec Map", &specularMapEnabled );
			pMC.specularMapEnabled = specularMapEnabled ? TRUE : FALSE;

			bool hasGlossMap = (bool)pMC.hasGlossMap;
			ImGui::Checkbox( "Gloss Alpha", &hasGlossMap );
			pMC.hasGlossMap = hasGlossMap ? TRUE : FALSE;

			ImGui::SliderFloat( "Spec Weight", &pMC.specularMapWeight, 0.0f, 2.0f );

			ImGui::SliderFloat( "Spec Pow", &pMC.specularPower, 0.0f, 1000.0f, "%f", 5.0f );

			ImGui::ColorPicker3( "Spec Color", reinterpret_cast<float*>( &pMC.specularColor ) );

			pcb->Update( gfx, pMC );

			return true;
		}
	}
	else if constexpr( std::is_same<PSMaterialConstantNoTex, M>::value )
	{
		if( auto pcb = meshPtrs.front()->QueryBindable<PixelConstantBuffer<M>>() )
		{
			ImGui::Text( "Material" );

			ImGui::ColorPicker3( "Spec Color", reinterpret_cast<float*>( &pMC.specularColor ) );

			ImGui::SliderFloat( "Spec Power", &pMC.specularPower, 0.0f, 1000.0f, "%f", 5.0f );

			ImGui::ColorPicker3( "Diffuse Color", reinterpret_cast<float*>( &pMC.materialColor ) );

			pcb->Update( gfx, pMC );

			return true;
		}
	}

	return false;
}