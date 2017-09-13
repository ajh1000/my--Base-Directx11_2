#pragma once
class UIcrosshair :
	public GameUIObject
{
public:
	UIcrosshair();
	virtual ~UIcrosshair(); 
	
	void init(int xpos, int ypos, int vertexWidth, int vertexHeight,
		int textureWidth, int textureHeight, string URL);
	virtual void update();
	virtual void render();
};

