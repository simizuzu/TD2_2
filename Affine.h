#pragma once

#include "WorldTransform.h"
#include "PrimitiveDrawer.h"

namespace Affine
{
	//�x���烉�W�A���ɕϊ�����萔
	constexpr float Deg2Rad = 3.1415926535897931f / 180.0f;
	constexpr float PIHalf = 3.1415926535897931f / 2.0f;

	/// <summary>
	/// �P�ʍs��
	/// </summary>
	/// <param name="matrix"></param>
	void CreateMatIdentity(Matrix4& matrix);

	/// <summary>
	/// �X�P�[��
	/// </summary>
	/// <param name="matrix"></param>
	/// <param name="scale"></param>
	void CreateMatScale(Matrix4& matrix, Vector3 scale);

	/// <summary>
	/// ��]
	/// </summary>
	/// <param name="matrix"></param>
	/// <param name="rot"></param>
	void CreateMatRot(Matrix4& matrix, Vector3 rot);

	/// <summary>
	/// ���s�ړ�
	/// </summary>
	/// <param name="matrix"></param>
	/// <param name="trans"></param>
	void CreateMatTrans(Matrix4& matrix, Vector3 trans);

	void CreateAffine(WorldTransform& worldTransform);

	/// <summary>
	/// �x�N�g���ƍs��̊|���Z
	/// </summary>
	/// <param name="matrix"></param>
	/// <param name="vector"></param>
	/// <returns></returns>
	Vector3 MatVector(Matrix4 matrix, Vector3 vector);

	Vector3 GetWorldTransform(Matrix4 matrix);
}
