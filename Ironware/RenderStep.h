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
#include "TechniqueProbe.h"

#include <vector>

class RenderQueuePass;
class RenderGraph;

/**
 * @brief Class that is responsible for submitting steps of the object rendering
 * * to the frame executor
*/
class RenderStep
{
public:
	RenderStep( std::string targetPassName );
	RenderStep( RenderStep&& ) = default;
	RenderStep( const RenderStep & src ) noexcept;
	RenderStep& operator=( const RenderStep& ) = delete;
	RenderStep& operator=( RenderStep&& ) = delete;
	void AddBindable( std::shared_ptr<Bindable> bind_in ) noexcept { bindables.push_back( std::move( bind_in ) ); }
	void Submit( const class Drawable& drawable ) const;
	void Bind( Graphics& gfx ) const IFNOEXCEPT;
	void InitializeParentReferences( const class Drawable& parent ) noexcept;
	void Accept( TechniqueProbe& probe );
	void Link( RenderGraph& rg );

protected:
	/**
	 * @brief Retrieves specified bindable from binds collection
	 * @tparam T required bindable type
	 * @return pointer to the bindable type, if it was found
	*/
	template<typename B>
	B* QueryBindable() const;

private:
	std::vector<std::shared_ptr<Bindable>> bindables;
	RenderQueuePass* pTargetPass = nullptr;
	std::string targetPassName;
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