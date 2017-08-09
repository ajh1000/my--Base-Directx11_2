#include "stdafx.h"
#include "testhtml5.h"


testhtml5::testhtml5()
{
	web_core = WebCore::Initialize(WebConfig()); 
	view = web_core->CreateWebView(WIDTH, HEIGHT);
	
	WebURL url(WSLit(URL));
	view->LoadURL(url); 

	while (view->IsLoading())
		web_core->Update();

	// Sleep a bit and update once more to give scripts and plugins
	// on the page a chance to finish loading.
	Sleep(300);
	web_core->Update(); 
	
	BitmapSurface* surface = (BitmapSurface*)view->surface();
	

	// Make sure our surface is not NULL-- it may be NULL if the WebView 
	// process has crashed.
	if (surface != 0) {
		// Save our BitmapSurface to a JPEG image in the current
		// working directory.
		surface->SaveToJPEG(WSLit("./result.jpg"));
	}
	
}


testhtml5::~testhtml5()
{
	view->Destroy();

	WebCore::Shutdown();
}
