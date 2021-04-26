/*!
 * \file BlendState.h
 *
 * \author Yernar Aldabergenov
 * \date April 2021
 *
 *
 */
#pragma once

#include "Bindable.h"
#include "BindableCollection.h"

#include <optional>
#include <array>

 /**
  * @brief Responsible class for managing blending
 */
class BlendState : public Bindable
{
public:
	BlendState( Graphics& gfx, bool isBlending, std::optional<float> factor = {} );
	void Bind( Graphics& gfx ) noexcept override;
	void SetFactor( float factor ) IFNOEXCEPT;
	float GetFactor() const IFNOEXCEPT;
	static std::shared_ptr<BlendState> Resolve( Graphics& gfx, bool isBlending, std::optional<float> factor = {} ) { return BindableCollection::Resolve<BlendState>( gfx, isBlending, factor ); }
	static std::wstring GenerateUID( bool isBlending, std::optional<float> factor );
	std::wstring GetUID() const noexcept override { return GenerateUID( isBlending, blendFactors->front() ); }

private:
	Microsoft::WRL::ComPtr<ID3D11BlendState> pBlendState;
	bool isBlending;
	std::optional<std::array<float, 4>> blendFactors;
};