/*!
 * \file PrimitiveTopology.h
 * \date 2020/09/23 5:43
 *
 * \author Yernar Aldabergenov
 * Contact: yernar.aa@gmail.com
 *
 * \brief A header that contains a (bindable) primitivetopology class
 *
*/
#pragma once

#include "Bindable.h"
#include "BindableCollection.h"
#include "IronUtils.h"

#include <typeinfo>

/*!
 * \class PrimitiveTopology
 *
 * \ingroup Bindables
 *
 * \brief A PrimitiveTopology wrapper child class that will be bound in process of graphics pipeline.
 *
 * \author Yernar Aldabergenov
 *
 * \date September 2020
 *
 * Contact: yernar.aa@gmail.com
 *
 */
class PrimitiveTopology : public Bindable
{
public:
	PrimitiveTopology( Graphics& gfx, D3D11_PRIMITIVE_TOPOLOGY type = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );

	void Bind( Graphics& gfx ) noexcept override { GetContext( gfx )->IASetPrimitiveTopology( type ); }
	static std::shared_ptr<Bindable> Resolve( Graphics& gfx, D3D11_PRIMITIVE_TOPOLOGY type ) { return BindableCollection::Resolve<PrimitiveTopology>( gfx, type ); }
	static std::wstring GenerateUID( D3D11_PRIMITIVE_TOPOLOGY type ) { return GET_CLASS_WNAME( PrimitiveTopology ) + L"#" + std::to_wstring( type ); }
	std::wstring GetUID() const noexcept override { return GenerateUID( type ); }

protected:
	D3D11_PRIMITIVE_TOPOLOGY type;
};