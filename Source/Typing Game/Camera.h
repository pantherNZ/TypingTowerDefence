#ifndef _CCAMERACLASS_H_
#define _CCAMERACLASS_H_

// Library Includes:

// Local Includes:
#include "Utility.h"

// Class Declaration:
class CCamera
{
	// class functions
	public:
		CCamera();
		~CCamera();

		void Initialise(D3DXVECTOR3 _vecPosition, D3DXVECTOR3 _vecAimPosition, D3DXVECTOR3 _vecUpDir, int _iWidth, int _iHeight);

		// get methods
		D3DXVECTOR3 GetPosition() const;
		D3DXMATRIX GetViewMatrix() const;
		D3DXMATRIX GetProjectionMatrix() const;

	protected:

	private:

	// class members
	public:

	protected:

	private:
		float m_fWidth;
		float m_fHeight;
		float m_fAspectRatio;

		// vectors to create view matrix
		D3DXVECTOR3 m_vecUp;
		D3DXVECTOR3 m_vecPosition;
		D3DXVECTOR3 m_vecAimPosition;
		float m_fRotation;

		// view matrix (camera class revolves around this)
		D3DXMATRIX m_matView;
		D3DXMATRIX m_matProjection;

		float m_fMoveSpeed;
};

#endif // _CCAMERACLASS_H_