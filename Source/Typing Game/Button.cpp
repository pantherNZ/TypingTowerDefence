// Self Include:
#include "Button.h"

// Local Includes:

// Library Includes:

// Static variables
bool CButton::m_bMouseDown(false);
int CButton::m_iMousePosX = 0;
int CButton::m_iMousePosY = 0;

// Implementation:
CButton::CButton() : m_bHighlightEnabled(false), m_bHighlight(false)
{

}

CButton::~CButton()
{

}

void CButton::SetMouseState(bool _bMouseDown)
{
	m_bMouseDown = _bMouseDown;
}

void CButton::SetMousePosition(int _iX, int _iY)
{
	m_iMousePosX = _iX;
	m_iMousePosY = _iY;
}

void CButton::Initialise(LPCWSTR _pstrTexture, D3DXVECTOR3 _vec3Position, D3DXVECTOR3 _vec3Rotation, D3DXVECTOR3 _vec3Scale, D3DXVECTOR2 _vec2CollisionBox)
{
	Initialise(_pstrTexture, _vec3Position, _vec3Rotation, _vec3Scale, _vec2CollisionBox, D3DXVECTOR4(0.0f, 0.0f, 1.0f, 1.0f));
}

void CButton::Initialise(LPCWSTR _pstrTexture, D3DXVECTOR3 _vec3Position, D3DXVECTOR3 _vec3Rotation, D3DXVECTOR3 _vec3Scale, D3DXVECTOR2 _vec2CollisionBox, D3DXVECTOR4 _vec4TexCoord)
{
	CSprite::Initialise(_pstrTexture, _vec3Position, _vec3Rotation, _vec3Scale, _vec4TexCoord);

	m_vec2CollisionBox = _vec2CollisionBox;
	m_bHighlight = false;
}

void CButton::Initialise(LPCWSTR _pstrTexture, D3DXVECTOR3 _vec3Position, D3DXVECTOR3 _vec3Rotation, D3DXVECTOR3 _vec3Scale, D3DXVECTOR2 _vec2CollisionBox, D3DXVECTOR4 _vec4TexCoord, D3DXVECTOR4 _vec4HighlightTexCoord)
{
	CSprite::Initialise(_pstrTexture, _vec3Position, _vec3Rotation, _vec3Scale, _vec4TexCoord);

	m_vec4TexCoordOriginal = _vec4TexCoord;
	m_vec2CollisionBox = _vec2CollisionBox;
	m_vec4HighlightTexCoord = _vec4HighlightTexCoord;
	m_bHighlightEnabled = true;
}

bool CButton::Process(float _fTime)
{
	// mouse collision
	if(m_iMousePosX >= m_vec3Position.x - m_vec2CollisionBox.x && m_iMousePosX <= m_vec3Position.x + m_vec2CollisionBox.x &&
		m_iMousePosY >= m_vec3Position.z - m_vec2CollisionBox.y && m_iMousePosY <= m_vec3Position.y + m_vec2CollisionBox.y)
	{
		if(!m_bHighlight && m_bHighlightEnabled)
		{
			m_bHighlight = true;
			SetTexCoord(m_vec4HighlightTexCoord);
		}
	
		if(m_bMouseDown)
		{
			m_bMouseDown = false;
			return(true);
		}	
	}	
	else if(m_bHighlight && m_bHighlightEnabled)
	{
		SetTexCoord(m_vec4TexCoordOriginal);
		m_bHighlight = false;
	}

	return(false);
}