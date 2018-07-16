// Self Include:
#include "Sprite.h"

// Local Includes:

// Library Includes:

// Static variables
ID3D10Device* CSprite::m_spDevice = nullptr;
ID3D10Buffer* CSprite::m_spIndexBuffer = nullptr;
ID3D10Buffer* CSprite::m_spVertexBuffer = nullptr;
int CSprite::m_siFaceCount = 2;

// Implementation:
CSprite::CSprite() : m_pTexture(nullptr), m_iTextureWidth(0), m_iTextureHeight(0), m_fAnimationInterval(0.0f), m_bAnimationEnabled(false), m_fTimer(0.0f), m_iAnimationRow(0)
{										  
	D3DXMatrixIdentity(&m_matWorld);
	m_vec2TexIncr = D3DXVECTOR2(0.0f, 0.0f);
	m_vec2SpriteSize = D3DXVECTOR2(0.0f, 0.0f);

	m_vec4TexCoord = D3DXVECTOR4(0.0f, 0.0f, 1.0f, 1.0f);
	m_vec3Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vec3Scale    = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vec3Rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

CSprite::~CSprite()
{
	if(m_pTexture != nullptr)
	{
		ReleaseCOM(m_pTexture);
	}
}

void CSprite::Initialise(LPCWSTR _pstrTexture, D3DXVECTOR3 _vec3Position, D3DXVECTOR3 _vec3Rotation, D3DXVECTOR3 _vec3Scale)
{
	Initialise(_pstrTexture, _vec3Position, _vec3Rotation, _vec3Scale, D3DXVECTOR4(0.0f, 0.0f, 1.0f, 1.0f));
}

void CSprite::Initialise(LPCWSTR _pstrTexture, D3DXVECTOR3 _vec3Position, D3DXVECTOR3 _vec3Rotation, D3DXVECTOR3 _vec3Scale, D3DXVECTOR4 _vec4TexCoord)
{
	SetPosition(_vec3Position);
	SetScale(_vec3Scale);
	m_vec3Rotation = _vec3Rotation;
	m_vec4TexCoord = _vec4TexCoord;

	if(FAILED(D3DX10CreateShaderResourceViewFromFile(m_spDevice, _pstrTexture, 0, 0, &m_pTexture, 0)))
	{
		MessageBox(NULL, L"ERROR: Initialising Sprite Texture Failed", _pstrTexture, MB_OK);	
		PostQuitMessage(0);
		return;
	}

	ID3D10Resource* pResource;
	m_pTexture->GetResource(&pResource);

	D3D10_TEXTURE2D_DESC desc2D;
	((ID3D10Texture2D*)pResource)->GetDesc(&desc2D);

	m_iTextureWidth = desc2D.Width;
	m_iTextureHeight = desc2D.Height;

	m_vec3Scale.z *= m_iTextureWidth;
	m_vec3Scale.x *= m_iTextureHeight;
	m_vec3Scale /= 100.0f;

	m_bAnimationEnabled = false;

	pResource->Release();

	BuildWorldMatrix();
	BuildTextureMatrix();
}

void CSprite::Initialise(LPCWSTR _pstrTexture, D3DXVECTOR3 _vec3Position, D3DXVECTOR3 _vec3Rotation, D3DXVECTOR3 _vec3Scale, float _fAnimationInterval, D3DXVECTOR2 _vec2SpriteSize)
{
	Initialise(_pstrTexture, _vec3Position, _vec3Rotation, _vec3Scale);

	m_bAnimationEnabled = true;
	m_fAnimationInterval = _fAnimationInterval / 1000.0f;
	m_vec2SpriteSize = _vec2SpriteSize;

	m_vec2TexIncr.x = (1.0f / m_iTextureWidth) * m_vec2SpriteSize.x;
	m_vec2TexIncr.y = (1.0f / m_iTextureHeight) * m_vec2SpriteSize.y;

	ResetTextureCoords();

	NormalSize();
	
	BuildTextureMatrix();
}	 

void CSprite::ResetTextureCoords()
{
	SetTexCoord(D3DXVECTOR4(0.001f, 0.001f, m_vec2TexIncr.x, m_vec2TexIncr.y));
}

void CSprite::NormalSize()
{
	m_vec3Scale *= 100.0f;
	m_vec3Scale.z /= m_iTextureWidth;
	m_vec3Scale.x /= m_iTextureHeight;

	BuildWorldMatrix();
}

void CSprite::CreateBuffers(ID3D10Device* _pDevice)
{
	m_spDevice = _pDevice;

	int iNumVertices = 4;

	// Indices
	DWORD* pIndices = new DWORD[m_siFaceCount * 3];

	pIndices[0] = 0;
	pIndices[1] = 1;
	pIndices[2] = 2;
	pIndices[3] = 2;
	pIndices[4] = 1;
	pIndices[5] = 3;

	// buffer desc
	D3D10_BUFFER_DESC indexBufferDesc;

	indexBufferDesc.Usage = D3D10_USAGE_IMMUTABLE;
	indexBufferDesc.ByteWidth = sizeof(DWORD) * m_siFaceCount * 3;
	indexBufferDesc.BindFlags = D3D10_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;

	D3D10_SUBRESOURCE_DATA indexInitialData;
	indexInitialData.pSysMem = pIndices;

	if(FAILED(m_spDevice->CreateBuffer(&indexBufferDesc, &indexInitialData, &m_spIndexBuffer)))
	{
		MessageBox(NULL, L"Initialising Index Buffer Failed", L"Error", MB_OK);	
		PostQuitMessage(0);
		return;
	}

	Vertex* pVertices = new Vertex[iNumVertices];

	pVertices[0].vecPosition = D3DXVECTOR3(-1.0f, 0.0f, -1.0f);
	pVertices[0].vecTexCoord = D3DXVECTOR2(1.0f, 1.0f);

	pVertices[1].vecPosition = D3DXVECTOR3(1.0f, 0.0f, -1.0f);
	pVertices[1].vecTexCoord = D3DXVECTOR2(1.0f, 0.0f);

	pVertices[2].vecPosition = D3DXVECTOR3(-1.0f, 0.0f, 1.0f);
	pVertices[2].vecTexCoord = D3DXVECTOR2(0.0f, 1.0f);

	pVertices[3].vecPosition = D3DXVECTOR3(1.0f, 0.0f, 1.0f);
	pVertices[3].vecTexCoord = D3DXVECTOR2(0.0f, 0.0f);

	// buffer settings (desc)
	D3D10_BUFFER_DESC vertexBufferDesc;

	vertexBufferDesc.Usage = D3D10_USAGE_IMMUTABLE;
	vertexBufferDesc.ByteWidth = sizeof(Vertex) * iNumVertices;
	vertexBufferDesc.BindFlags = D3D10_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;

	D3D10_SUBRESOURCE_DATA vertexInitialData;
	vertexInitialData.pSysMem = pVertices;

	if(FAILED(m_spDevice->CreateBuffer(&vertexBufferDesc, &vertexInitialData, &m_spVertexBuffer)))
	{
		MessageBox(NULL, L"Initialising Vertex Buffer Failed", L"Error", MB_OK);	
		PostQuitMessage(0);
		return;
	}

	delete[] pIndices;
	delete[] pVertices;
}

void CSprite::ReleaseBuffers()
{
	ReleaseCOM(m_spVertexBuffer);
	ReleaseCOM(m_spIndexBuffer);
}

int CSprite::GetFaceCount()
{
	return(m_siFaceCount);
}

void CSprite::ApplyRenderOptions()
{
	UINT uiStride = sizeof(Vertex);
	UINT uiOffset = 0;

	// set buffers to be used
	m_spDevice->IASetVertexBuffers(0, 1, &m_spVertexBuffer, &uiStride, &uiOffset);
	m_spDevice->IASetIndexBuffer(m_spIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
}

void CSprite::Process(float _fTime)
{
	if(m_bAnimationEnabled)
	{
		if(_fTime > m_fTimer + m_fAnimationInterval)
		{
			m_fTimer = _fTime;				

			if(m_vec4TexCoord.z + m_vec2TexIncr.x >= 1.01f)
			{
				float fY = float(m_vec2SpriteSize.y / m_iTextureHeight) * m_iAnimationRow;
				SetTexCoord(D3DXVECTOR4(0.001f, fY + 0.001f, m_vec2TexIncr.x, fY + m_vec2TexIncr.y));
			}
			else
			{
				TranslateTexCoord(D3DXVECTOR4(m_vec2TexIncr.x, 0.0f, m_vec2TexIncr.x, 0.0f));
			}
		}
	}
}

ID3D10ShaderResourceView* CSprite::GetTexture() const
{
	return(m_pTexture);
}

void CSprite::SetPosition(D3DXVECTOR3 _vec3Position)
{
	m_vec3Position.x = _vec3Position.z;
	m_vec3Position.y = _vec3Position.y;
	m_vec3Position.z = -_vec3Position.x;
	BuildWorldMatrix();
}

D3DXVECTOR3 CSprite::GetPosition() const
{
	return(m_vec3Position);
}

void CSprite::SetRotation(D3DXVECTOR3 _vec3Rotation)
{
	m_vec3Rotation = _vec3Rotation;
	BuildWorldMatrix();
}

void CSprite::Move(D3DXVECTOR3 _vec3Position)
{
	m_vec3Position.x += _vec3Position.z;
	m_vec3Position.y += _vec3Position.y;
	m_vec3Position.z += -_vec3Position.x;
	BuildWorldMatrix();
}

void CSprite::Rotate(D3DXVECTOR3 _vec3Rotation)
{
	m_vec3Rotation += _vec3Rotation;
	BuildWorldMatrix();
}

D3DXVECTOR3 CSprite::GetRotation() const
{
	return(m_vec3Rotation);
}

void CSprite::SetScale(D3DXVECTOR3 _vec3Scale)
{
	m_vec3Scale.x = _vec3Scale.z;
	m_vec3Scale.y = _vec3Scale.y;
	m_vec3Scale.z = _vec3Scale.x;
	BuildWorldMatrix();
}

void CSprite::ModifyScale(D3DXVECTOR3 _vec3Scale)
{
	m_vec3Scale.x += _vec3Scale.z;
	m_vec3Scale.y += _vec3Scale.y;
	m_vec3Scale.z += _vec3Scale.x;
	BuildWorldMatrix();
}

D3DXVECTOR3 CSprite::GetScale() const
{
	return(m_vec3Scale);
}

D3DXMATRIX CSprite::GetWorldMatrix() const
{
	return(m_matWorld);
}

D3DXMATRIX CSprite::GetTextureMatrix() const
{
	return(m_matTexture);
}

int CSprite::GetWidth() const
{
	return(m_iTextureWidth);
}

int CSprite::GetHeight() const
{
	return(m_iTextureHeight);
}

void CSprite::SetTexCoord(D3DXVECTOR4 _vec4TexCoord)
{
	m_vec4TexCoord = _vec4TexCoord;
	BuildTextureMatrix();
}

D3DXVECTOR4 CSprite::GetTexCoord() const
{
	return(m_vec4TexCoord);
}

void CSprite::TranslateTexCoord(D3DXVECTOR4 _vec4TexCoord)
{
	m_vec4TexCoord += _vec4TexCoord;
	BuildTextureMatrix();
}

void CSprite::BuildWorldMatrix()
{
	D3DXMATRIX matTranslation = *D3DXMatrixTranslation(&matTranslation, m_vec3Position.x, m_vec3Position.y, m_vec3Position.z);
	D3DXMATRIX matRotation = *D3DXMatrixRotationYawPitchRoll(&matRotation, m_vec3Rotation.x, m_vec3Rotation.y, m_vec3Rotation.z);
	D3DXMATRIX matScaling = *D3DXMatrixScaling(&matScaling, m_vec3Scale.x, m_vec3Scale.y, m_vec3Scale.z);

	m_matWorld = matScaling * matRotation * matTranslation;
}

void CSprite::BuildTextureMatrix()
{
	D3DXMATRIX matTranslation = *D3DXMatrixTranslation(&matTranslation, m_vec4TexCoord.x, m_vec4TexCoord.y, 0.0f);
	D3DXMATRIX matScaling = *D3DXMatrixScaling(&matScaling, m_vec4TexCoord.z - m_vec4TexCoord.x, m_vec4TexCoord.w - m_vec4TexCoord.y, 1.0f);

	m_matTexture = matScaling * matTranslation;
}

float CSprite::GetAnimationInterval() const
{
	return ((m_fAnimationInterval * (m_iTextureWidth / m_vec2SpriteSize.x)) * 1000.0f);
}

/*
float CSprite::GetAnimationInterval() const
{
	return (m_fAnimationInterval * 4);
}
*/
void CSprite::SetAnimationInterval(float _fInterval)
{
	m_fAnimationInterval = _fInterval / 1000.0f;
}