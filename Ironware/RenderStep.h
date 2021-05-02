/*!
 * \file RenderStep.h
 *
 * \author Yernar Aldabergenov
 * \date May 2021
 *
 * 
 */
#pragma once

#include "Graphics.h"
#include "Bindable.h"

#include <vector>

/**
 * @brief 
*/
class RenderStep
{
public:
	RenderStep( size_t targetPass_in );
	void AddBindable( std::shared_ptr<Bindable> bind_in ) noexcept { bindables.push_back( std::move( bind_in ) ); }
	void Submit( class FrameExecutor& frame, const class Drawable& drawable ) const;
	void Bind( Graphics& gfx ) const;
	void InitializeParentReferences( const class Drawable& parent ) noexcept;

protected:
	/**
	 * @brief Retrieves specified bindable from binds collection
	 * @tparam T required bindable type
	 * @return pointer to the bindable type, if it was found
	*/
	template<typename B>
	B* QueryBindable() const;

private:
	size_t targetPass;
	std::vector<std::shared_ptr<Bindable>> bindables;
};

template<typename B>
B* RenderStep::QueryBindable() const
{
	for( const auto& pb : bindables )
	{
		if( auto pT = dynamic_cast<B*>( pb.get() ) )
		{
			return pT;
		}
	}
	return nullptr;
}