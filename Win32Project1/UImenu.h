#pragma once
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
};

