/*!
 * \file Sampler.h
 * \date 2020/11/06 14:20
 *
 * \author Yernar Aldabergenov
 * Contact: yernar.aa@gmail.com
 *
 * \brief This file contains a (bindable) sampler class.
 *
*/
#pragma once

#include "Bindable.h"
#include "BindableCollection.h"
#include "IronUtils.h"

#include <typeinfo>

/*!
 * \class Sampler
 *
 * \ingroup Bindables
 *
 * \brief A class that is is responsible for setting lookup configs.
 *
 * \author Yernar Aldabergenov
 *
 * \date September 2020
 *
 * Contact: yernar.aa@gmail.com
 *
 */
class Sampler : public Bindable
{
public:
	enum class Type
	{
		Anisotropic,
		Bilinear,
		Point,
	};

public:
	Sampler( Graphics& gfx, Type type, bool reflect, UINT slot = 0u );
	void Bind( Graphics& gfx ) IFNOEXCEPT override;
	static std::shared_ptr<Sampler> Resolve( Graphics& gfx, Type type = Type::Anisotropic, bool reflect = false, UINT slot = 0u ) { return BindableCollection::Resolve<Sampler>( gfx, type, reflect, slot ); }
	static std::wstring GenerateUID( Type type, bool reflect, UINT slot );
	std::wstring GetUID() const noexcept override { return GenerateUID( type, reflection, slot ); }

protected:
	Microsoft::WRL::ComPtr<ID3D11SamplerState> pSampler;
	const UINT slot;
	Type type;
	bool reflection;
};