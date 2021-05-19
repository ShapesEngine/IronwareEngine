#pragma once

#include "CommonMacros.h"

#include <string>
#include <vector>
#include <memory>

class Pass;
class RenderQueuePass;
class Source;
class Sink;
class Graphics;
class RenderTarget;
class DepthStencilView;

class RenderGraph
{
public:
	RenderGraph( Graphics& gfx );
	~RenderGraph();
	void Execute( Graphics& gfx ) IFNOEXCEPT;
	void Reset() noexcept;
	RenderQueuePass& GetRenderQueue( const std::string& passName );
	void StoreDepth( Graphics& gfx, const std::wstring& path );

protected:
	void SetSinkTarget( const std::string& sinkName, const std::string& target );
	void AddGlobalSource( std::unique_ptr<Source> );
	void AddGlobalSink( std::unique_ptr<Sink> );
	void Finalize();
	void AppendPass( std::unique_ptr<Pass> pass );

private:
	void LinkSinks( Pass& pass );
	void LinkGlobalSinks();

private:
	std::vector<std::unique_ptr<Pass>> passes;
	std::vector<std::unique_ptr<Source>> globalSources;
	std::vector<std::unique_ptr<Sink>> globalSinks;
	std::shared_ptr<RenderTarget> backBufferTarget;
	std::shared_ptr<DepthStencilView> masterDepth;
	bool finalized = false;
};