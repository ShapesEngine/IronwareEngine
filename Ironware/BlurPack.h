/*!
 * \file BlurPack.h
 *
 * \author Yernar Aldabergenov
 * \date May 2021
 *
 *
 */
#pragma once

#include "BindableCommon.h"
#include "IronMath.h"

#include <imgui/imgui.h>

class BlurPack
{
public:
	BlurPack( Graphics& gfx, int radius = 7, float sigma = 2.6f, const wchar_t* shader = L"Filter_PS.cso" ) :
		shader( gfx, shader ),
		pcb( gfx, 0u ),
		ccb( gfx, 1u ),
		radius( radius ),
		sigma( sigma )
	{
		SetKernelGauss( gfx, radius, sigma );
	}
	void Bind( Graphics& gfx ) IFNOEXCEPT
	{
		shader.Bind( gfx );
		pcb.Bind( gfx );
		ccb.Bind( gfx );
	}
	void SetHorizontal( Graphics& gfx )
	{
		ccb.Update( gfx, { TRUE } );
	}
	void SetVertical( Graphics& gfx )
	{
		ccb.Update( gfx, { FALSE } );
	}
	void ShowWindow( Graphics& gfx )
	{
		ImGui::Begin( "Blur" );

		bool filterChanged = false;
		{
			const char* items[] = { "Gauss","Box" };
			static const char* curItem = items[0];
			if( ImGui::BeginCombo( "Filter Type", curItem ) )
			{
				for( int n = 0; n < std::size( items ); n++ )
				{
					const bool isSelected = ( curItem == items[n] );
					if( ImGui::Selectable( items[n], isSelected ) )
					{
						filterChanged = true;
						curItem = items[n];
						if( curItem == items[0] )
						{
							kernelType = KernelType::Gauss;
						}
						else if( curItem == items[1] )
						{
							kernelType = KernelType::Box;
						}
					}
					if( isSelected )
					{
						ImGui::SetItemDefaultFocus();
					}
				}
				ImGui::EndCombo();
			}
		}

		bool radChange = ImGui::SliderInt( "Radius", &radius, 0, 15 );
		bool sigChange = ImGui::SliderFloat( "Sigma", &sigma, 0.1f, 10.0f );
		if( radChange || sigChange || filterChanged )
		{
			if( kernelType == KernelType::Gauss )
			{
				SetKernelGauss( gfx, radius, sigma );
			}
			else if( kernelType == KernelType::Box )
			{
				SetKernelBox( gfx, radius );
			}
		}
		ImGui::End();
	}
	void SetKernelGauss( Graphics& gfx, int radius, float sigma ) IFNOEXCEPT
	{
		assert( radius <= MAX_RADIUS );
		Kernel k;
		k.nTaps = radius * 2 + 1;
		float sum = 0.0f;
		for( int i = 0; i < k.nTaps; i++ )
		{
			const auto x = float( i - radius );
			const auto g = gauss( x, sigma );
			sum += g;
			k.coefficients[i].x = g;
		}
		for( int i = 0; i < k.nTaps; i++ )
		{
			k.coefficients[i].x /= sum;
		}
		pcb.Update( gfx, k );
	}
	void SetKernelBox( Graphics& gfx, int radius ) IFNOEXCEPT
	{
		assert( radius <= MAX_RADIUS );
		Kernel k;
		k.nTaps = radius * 2 + 1;
		const float c = 1.0f / k.nTaps;
		for( int i = 0; i < k.nTaps; i++ )
		{
			k.coefficients[i].x = c;
		}
		pcb.Update( gfx, k );
	}
private:
	enum class KernelType
	{
		Gauss,
		Box
	};
	static constexpr int MAX_RADIUS = 15;
	int radius;
	float sigma;
	KernelType kernelType = KernelType::Gauss;
	struct Kernel
	{
		alignas( 16 ) int nTaps;
		DirectX::XMFLOAT4 coefficients[MAX_RADIUS * 2 + 1];
	};

	struct Control
	{
		alignas( 16 ) BOOL horizontal;
	};

	PixelShader shader;
	PixelConstantBuffer<Kernel> pcb;
	PixelConstantBuffer<Control> ccb;
};