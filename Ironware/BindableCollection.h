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
	/**
	 * @brief Function that resolves bindable type and either stores some value
	 * * or gets the existing value from the container
	 * @tparam T Type of the Bindable
	 * @tparam TPACK Parameter pack value
	 * @param gfx Needed to instantiate T
	 * @return shared_ptr to the resolved type
	*/
	template<class T, TPACK Params>
	static std::shared_ptr<Bindable> Resolve( Graphics& gfx, Params&&... p ) IFNOEXCEPT;

private:
	template<class T, TPACK Params>
	std::shared_ptr<Bindable> Resolve_( Graphics& gfx, Params&&... p ) IFNOEXCEPT;
	static BindableCollection& Get() noexcept;

private:
	std::unordered_map<std::wstring, std::shared_ptr<Bindable>> bindables;
};

#pragma region implementation

template<class T, TPACK Params>
std::shared_ptr<Bindable> BindableCollection::Resolve( Graphics & gfx, Params && ...p ) noexcept( !IS_DEBUG )
{
	static_assert( std::is_base_of<Bindable, T>::value, "Can only resolve classes derived from Bindable" );
	return Get().Resolve_<T>( gfx, std::forward<Params>( p )... );
}

template<class T, TPACK Params>
std::shared_ptr<Bindable> BindableCollection::Resolve_( Graphics & gfx, Params && ...p ) noexcept( !IS_DEBUG )
{
	const auto key = T::GenerateUID( std::forward<Params>( p )... );
	const auto i = bindables.find( key );
	if( i != bindables.cend() )
	{
		return i->second;
	}
	auto bind = std::make_shared<T>( gfx, std::forward<Params>( p )... );
	bindables[key] = bind;

	return bind;
}

inline BindableCollection& BindableCollection::Get() noexcept
{
	static BindableCollection collection;
	return collection;
}

#pragma endregion implementation