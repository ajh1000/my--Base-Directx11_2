#include "stdafx.h"
#include "UIhealthBar.h"
#include "gamePlayer.h"
#include "gameCameraThirdPerson.h"
UIhealthBar::UIhealthBar()
{
}


UIhealthBar::~UIhealthBar()
{
}

void UIhealthBar::init(int xpos, int ypos, int vertexWidth, int vertexHeight, int textureWidth, int textureHeight, string URL)
{
	GameUIObject::init(xpos, ypos, vertexWidth, vertexHeight, textureWidth, textureHeight, URL);

	
}

void UIhealthBar::update()
{

}

void UIhealthBar::render()
{
	
	GameUIObject::render();
}


void UIhealthBar::TakeDamage(float dmg)
{
	if (m_htmlTexture.m_view)
	{

		JSValue window = m_htmlTexture.m_view->ExecuteJavascriptWithResult(
			WSLit("window"), WSLit(""));

		if (window.IsObject()) {
			JSArray args;
			m_hp -= dmg;
			args.Push(JSValue(dmg));

			window.ToObject().Invoke(WSLit("damage"), args);
		}

		if (m_hp <= 0)
		{
			((gamePlayer*)gameUtil.m_mapTag["player"])->setAnimation("Idle");
			((gamePlayer*)gameUtil.m_mapTag["player"])->m_isDead = true;
			((gamePlayer*)gameUtil.m_mapTag["player"])->m_rigidbody->setLinearFactor(btVector3(1, 1, 1));
			((gamePlayer*)gameUtil.m_mapTag["player"])->m_rigidbody->setAngularFactor(btVector3(1, 1, 1));
			D3DXVECTOR3 forward = ((gameCameraThirdPerson*)gameUtil.GetMainCamera())->m_vForward;
			forward.y = 0;
			((gamePlayer*)gameUtil.m_mapTag["player"])->m_rigidbody->applyCentralImpulse(btVector3( forward.x,
				0,forward.z)*5);
		}
	}
}

void UIhealthBar::reset()
{
	if (m_htmlTexture.m_view)
	{

		JSValue window = m_htmlTexture.m_view->ExecuteJavascriptWithResult(
			WSLit("window"), WSLit(""));

		if (window.IsObject()) {
			JSArray args;
			m_hp = 1;

			window.ToObject().Invoke(WSLit("reset"), args);

		}
	}
}
