#pragma once
class MenuDispatcher : public JSMethodHandler
{
public:
	// JSMethodHandler을(를) 통해 상속됨
	virtual void OnMethodCall(Awesomium::WebView * caller, unsigned int remote_object_id, const Awesomium::WebString & method_name, const Awesomium::JSArray & args) override;
	virtual Awesomium::JSValue OnMethodCallWithReturnValue(Awesomium::WebView * caller, unsigned int remote_object_id, const Awesomium::WebString & method_name, const Awesomium::JSArray & args) override;
public:
	class UImenu* m_menu = nullptr;
};

class UImenu : public GameUIObject
{
public:
	UImenu();
	virtual ~UImenu();

public:
	void init(int xpos, int ypos, int vertexWidth, int vertexHeight,
		int textureWidth, int textureHeight, string URL);
	void update();
	void render();
private:
	MenuDispatcher m_dispatcher;
};

