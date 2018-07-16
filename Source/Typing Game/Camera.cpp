// Self Include:
#include "Camera.h"

// Local Includes:

// Library Includes:

// Implementation:
CCamera::CCamera() : m_fMoveSpeed(2.0f), m_fWidth(800.0f), m_fHeight(600.0f), m_fRotation(0.0f)
{
	m_vecUp = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_vecPosition = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	m_vecAimPosition = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
}

void CCamera::Initialise(D3DXVECTOR3 _vecPosition, D3DXVECTOR3 _vecAimPosition, D3DXVECTOR3 _vecUpDir, int _iWidth, int _iHeight)
{
	m_fWidth = (float)_iWidth;
	m_fHeight = (float)_iHeight;
	m_fAspectRatio = m_fWidth / m_fHeight;

	m_vecPosition = _vecPosition;
	m_vecAimPosition = _vecAimPosition;
	m_vecUp = _vecUpDir;

	D3DXMatrixLookAtLH(&m_matView, &m_vecPosition, &m_vecAimPosition, &m_vecUp);
	//D3DXMatrixOrthoLH(&m_matProjection, m_fWidth, m_fHeight, 0.0f, 1.0f);
	D3DXMatrixPerspectiveFovLH(&m_matProjection, (float)D3DX_PI / 4.0f, m_fAspectRatio, 0.001f, 1000.0f);
}

CCamera::~CCamera()
{
	
}

D3DXVECTOR3 CCamera::GetPosition() const
{
	return(m_vecPosition);
}

D3DXMATRIX CCamera::GetViewMatrix() const
{
	return(m_matView);
}

D3DXMATRIX CCamera::GetProjectionMatrix() const
{
	return(m_matProjection);
}