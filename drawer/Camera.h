#ifndef _CAMERA_H_
#define _CAMERA_H_
#include <D3DX10math.h>
#include "typedefs.h"
#include "arithmetic.h"
#include "Box3.h"

class Camera{

public:
	Camera();
	Camera(const Camera&);
	~Camera();

	void SetPosition(float, float, float);
	void SetRotation(float, float, float);

	D3DXVECTOR3 GetPosition();
	D3DXVECTOR3 GetRotation();

	void Render();
	void GetViewMatrix(D3DXMATRIX&);

	void Translate(float x, float y);
	void ScaleView(float k);
	float GetScale() const;
	void SetScale(float k);
private:
	float m_positionX, m_positionY, m_positionZ;
	float m_rotationX, m_rotationY, m_rotationZ;
	D3DXMATRIX m_viewMatrix;

	float mScale;
};

#endif 




