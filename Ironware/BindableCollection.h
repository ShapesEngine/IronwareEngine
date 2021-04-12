/*!
 * \file BindableCollection.h
 * \date 2021/04/12 21:33
 *
 * \author Yernar Aldabergenov
 * Contact: yernar.aa@gmail.com
 *
 * \brief Header that contains BindableCollection class
*/
#pragma once

#include "Bindable.h"
#include "CommonMacros.h"

#include <unordered_map>

/**
 * @brief Singleton container class which stores all of the bindables
 * * and enables sharing bindables between different type of drawables
*/
class BindableCollection
{
public:
	static std::shared_ptr<Bindable> Resolve( const std::wstring& key )
	{
		return Get().Resolve_( key );
	}

	static void Store( std::shared_ptr<Bindable> bindable )
	{
		Get().Store_( bindable );
	}

private:
	std::shared_ptr<Bindable> Resolve_( const std::wstring& key ) const IFNOEXCEPT
	{
		const auto i = bindables.find( key );
		if( i != bindables.cend() )
		{
			return i->second;
		}
		return{};
	}

	void Store_( std::shared_ptr<Bindable> bindable ) IFNOEXCEPT
	{
		bindables[bindable->GetUID()] = std::move( bindable );
	}

	static BindableCollection& Get() noexcept
	{
		static BindableCollection collection;
		return collection;
	}

private:
	std::unordered_map<std::wstring, std::shared_ptr<Bindable>> bindables;
};