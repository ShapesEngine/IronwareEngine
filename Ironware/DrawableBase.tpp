/*!
 * \file DrawableBase.tpp
 *
 * \author Yernar Aldabergenov
 * \date November 2020
 *
 * 
 */
template<class T>
std::vector<std::unique_ptr<Bindable>> DrawableBase<T>::staticBinds;

template<typename T>
void DrawableBase<T>::AddStaticBind( std::unique_ptr<Bindable> bind ) noexcept( !IS_DEBUG )
{
	assert( "*Must* use AddStaticIndexBuffer to bind index buffer" && typeid( *bind ) != typeid( IndexBuffer ) );
	staticBinds.push_back( std::move( bind ) );
}

template<typename T>
void DrawableBase<T>::AddStaticIndexBuffer( std::unique_ptr<class IndexBuffer> ibuf ) noexcept
{
	assert( "Attempting to add index buffer a second time" && pIndexBuffer == nullptr );
	pIndexBuffer = ibuf.get();
	staticBinds.push_back( std::move( ibuf ) );
}

template<typename T>
void DrawableBase<T>::SetIndexFromStatic() noexcept( !IS_DEBUG )
{
	assert( "Attempting to add index buffer a second time" && pIndexBuffer == nullptr );
	for( const auto& b : staticBinds )
	{
		if( const auto p = dynamic_cast<IndexBuffer*>( b.get() ) )
		{
			pIndexBuffer = p;
			return;
		}
	}
	assert( "Failed to find index buffer in static binds" && pIndexBuffer != nullptr );
}