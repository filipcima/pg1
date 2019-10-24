#include "stdafx.h"
#include "camera.h"

Camera::Camera( const int width, const int height, const float fov_y,
	const Vector3 view_from, const Vector3 view_at )
{
	width_ = width;
	height_ = height;
	fov_y_ = fov_y;

	view_from_ = view_from;
	view_at_ = view_at;

	// TODO compute focal lenght based on the vertical field of view and the camera resolution
	f_y_ = height_ / (2 * tan(fov_y_ / 2));

	// TODO build M_c_w_ matrix	
	Vector3 z_c = view_from_ - view_at_;
	z_c.Normalize();
	Vector3 x_c = up_.CrossProduct(z_c);
	x_c.Normalize();
	Vector3 y_c = z_c.CrossProduct(x_c);
	y_c.Normalize();

	M_c_w_ = Matrix3x3( x_c, y_c, z_c );
}

RTCRay Camera::GenerateRay( const float x_i, const float y_i ) const
{
	RTCRay ray = RTCRay();
	
	const float x = x_i - width_ / 2;
	const float y = height_ / 2 - y_i;

	// Ray direction in camera coords
	Vector3 direction = Vector3(x, y, -f_y_);
	direction.Normalize();

	// Ray direction in RL coords
	direction = M_c_w_ * direction;

	ray.org_x = view_from_.x;
	ray.org_y = view_from_.y;
	ray.org_z = view_from_.z;

	ray.dir_x = direction.x;
	ray.dir_y = direction.y;
	ray.dir_z = direction.z;

	ray.tnear = FLT_MIN;
	ray.tfar = FLT_MAX;
	ray.time = 0.0f;

	return ray;
}
