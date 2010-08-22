//------------------------------------------------------------------------------------
//	Core Class
//
//	2010. 08. 04	CoderK
//
//	Core�� �������̽� ����(����Ŭ����)�� �Ѵ�. ��� Core�� �긦 ��ӽ��Ѽ� �Ѵ�.
//	��� �Ŀ� �̵� �Լ��� ��� �����ؾ� ��.
//------------------------------------------------------------------------------------
#pragma once

class Core  
{
	virtual bool SetUp() = 0;			// virtual type func() = 0;  means ���������Լ�.
	virtual void MainLoop() = 0;
	virtual void Draw() = 0;
	virtual	void Release() = 0;

	//about mouse
	virtual void OnLButtonDown() = 0;
	virtual void OnLButtonUp() = 0;
	virtual void OnMouseMove() = 0;
	virtual void OnRButtonDown() = 0;
};