#pragma once


#include <Awesomium/WebCore.h>
#include <Awesomium/BitmapSurface.h>
#include <Awesomium/STLHelpers.h>


#define WIDTH   800
#define HEIGHT  600
#define URL "http://www.google.com"
//#define URL     "C:/Users/moula/Documents/workspace Java/mystudy/WebContent/main.jsp"
using namespace Awesomium;

class testhtml5
{
public:
	testhtml5();
	virtual ~testhtml5();
	WebCore* web_core;
	WebView* view;
};

