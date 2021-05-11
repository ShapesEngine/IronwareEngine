/*!
 * \file InputLayout.h
 * \date 2020/09/23 5:36
 *
 * \author Yernar Aldabergenov
 * Contact: yernar.aa@gmail.com
 *
 * \brief A header that contains a (bindable) inputlayout class
 *
*/
#pragma once

#include "Bindable.h"
#include "BindableCollection.h"
#include "IronUtils.h"
#include "Vertex.h"
#include "VertexShader.h"

/*!
 * \class InputLayout
 *
 * \ingroup Bindables
 *
 * \brief An InputLayout wrapper class that will be needed and bound in process of graphics pipeline
 * * and store vertex indices in it.
 *
 * \author Yernar Aldabergenov
 *
 * \date September 2020
 *
 * Contact: yernar.aa@gmail.com
 *
 */
class InputLayout : public Bindable
{
public:
	InputLayout( Graphics& gfx,
		const VertexLayout& layout_in,
		const VertexShader& vs );

	void Bind( Graphics& gfx ) IFNOEXCEPT override { GetContext( gfx )->IASetInputLayout( pInputLayout.Get() ); }
	static std::shared_ptr<InputLayout> Resolve( Graphics& gfx, const VertexLayout& layout, const VertexShader& vs ) { return BindableCollection::Resolve<InputLayout>( gfx, layout, vs ); }
	static std::wstring GenerateUID( const VertexLayout& layout, const VertexShader& vs ) { return GET_CLASS_WNAME( InputLayout ) + L"#" + layout.GetCode() + L"#" + vs.GetUID(); }
	std::wstring GetUID() const noexcept override { return GenerateUID( layout, *pVShader ); }

protected:
	const VertexShader* pVShader;
	const VertexLayout& layout;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> pInputLayout;
};