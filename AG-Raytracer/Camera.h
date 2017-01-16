#pragma once

#include "template.h"
//#include "utils.hpp"

/*
#include "glm/vec3.hpp"
#include "glm/gtc/constants.hpp"
#include "glm/glm.hpp"
#include "glm/gtx/epsilon.hpp">
#include "glm/gtx/rotate_vector.hpp"
#include "glm/gtx/vector_query.hpp"
*/

//#include <algorithm>
//#include <cassert>
//#include <cmath>
//#include <iostream>

#define DEFAULT_FOV (PI / 2)

// Based on code from Douwe van Gijn & Nick Begg
// # Thanks guys

class Camera {
public:
	Ray* primaryRays[SCRWIDTH*SCRHEIGHT];

	Camera() :
		mWidth(SCRWIDTH), //mWidth(0),
		mHeight(SCRHEIGHT),//mHeight(0),
		mStartingOrigin(1.0f, 1.0f, -1.0f),
		mStartingPitch(PI / 6),
		mStartingYaw(-PI / 6),
		mStartingFov(DEFAULT_FOV),
		mDirty(true)
	{
		resetView();
		buildLookVectors();
		buildCamera();
		sanityCheck();
		GenerateRays();
	}

	void setScreenRes(int width, int height)
	{
		mWidth = width;
		mHeight = height;
		buildCamera();
	}

	void resetView()
	{
		mOrigin = mStartingOrigin;
		mYaw = mStartingYaw;
		mPitch = mStartingPitch;
		mFov = mStartingFov;
		mDirty = true;

		buildLookVectors();
		buildCamera();
	}

	void setStartingValues(
		const glm::vec3& startingOrigin,
		float startingYaw,
		float startingPitch,
		float startingFov)
	{
		mStartingOrigin = startingOrigin;
		mStartingYaw = startingYaw;
		mStartingPitch = startingPitch;
		mStartingFov = startingFov;
	}

	bool dirty() const
	{
		return mDirty;
	}

	void clearDirty()
	{
		mDirty = false;
	}

	void sanityCheck() const
	{
		// check that the camera/screen has been set up - ie buildCamera has been called
		// ... this will fail if buildCamera's not been called.
		assert(glm::length(mU) > 0);
		assert(glm::length(mV) > 0);

		// u & v should be perpendicular
		auto dot = glm::dot(mU, mV);
		assert(dot < 0.0f + EPSILON || dot < 0.0f - EPSILON);
	}

	// takes a screen co-ord, and returns a ray from the camera through that pixel
	// note that this is in screen space and not world space, the conversion is handled internally
	Ray* makeRay(int x, int y) const
	{
		assert(x >= 0);
		assert(y >= 0);
		assert(x < mWidth);
		assert(y < mHeight);

		// check the width and height have been set
		assert(mWidth > 0);
		assert(mHeight > 0);

		sanityCheck();

		// convert pixel space to percent across the screen
		float px = ((float)x) / mWidth;
		float py = ((float)y) / mHeight;

		// using pre-existing u & v vectors, get a point on the screen in world space
		const glm::vec3 screenPoint = mTopLeft + (px * mU) + (py * mV);

		// and a vector from the eye to the screen point
		const glm::vec3 lookVec = screenPoint - mOrigin;

		// build result - note direction is normalised
		return new Ray(mOrigin, glm::normalize(lookVec));
	};

	void Camera::GenerateRays()
	{
		for (int y = 0; y < SCRHEIGHT; y++) {
			for (int x = 0; x < SCRWIDTH; x++)
			{
				primaryRays[y * SCRWIDTH + x] = makeRay(x,y);
			}
		}
	}

	void Camera::UpdateRays()
	{
		for (int y = 0; y < SCRHEIGHT; y++) {
			for (int x = 0; x < SCRWIDTH; x++)
			{
				// convert pixel space to percent across the screen
				float px = ((float)x) / mWidth;
				float py = ((float)y) / mHeight;

				// using pre-existing u & v vectors, get a point on the screen in world space
				const glm::vec3 screenPoint = mTopLeft + (px * mU) + (py * mV);

				// and a vector from the eye to the screen point
				const glm::vec3 lookVec = screenPoint - mOrigin;

				Ray* ray = primaryRays[y * SCRWIDTH + x];
				ray->direction = glm::normalize(lookVec);
				ray->origin = mOrigin;
				ray->t = INFINITY;
			}
		}
	}

	// FIXME: probably break this into a functon to move the origin and change the other params 
	// separately, as they are triggered from separate inputs
	// y/p/r must be <2pi, fov < pi
	void buildCamera()
	{
		assert(isAngleInOneRev(mYaw));
		assert(isAngleInOneRev(mPitch));
		assert(isAngleInHalfRev(mFov));

		const float aspectRatio = (float)mWidth / float(mHeight);

		// start with 3x points around the screen
		auto tl = glm::vec3(-aspectRatio, 1, 1);
		auto bl = glm::vec3(-aspectRatio, -1, 1);
		auto tr = glm::vec3(aspectRatio, 1, 1);

		// adjust for fov, keeping dist along z axis const
		float fov_ratio = glm::tan(mFov / 2);
		auto fov_adj = glm::vec3(fov_ratio, fov_ratio, 1);

		tl = tl * fov_adj;
		bl = bl * fov_adj;
		tr = tr * fov_adj;

		// pitch and yaw.. note if done in this order, doesn't require a composed translation
		tl = glm::rotateX(tl, mPitch);
		bl = glm::rotateX(bl, mPitch);
		tr = glm::rotateX(tr, mPitch);

		tl = glm::rotateY(tl, mYaw);
		bl = glm::rotateY(bl, mYaw);
		tr = glm::rotateY(tr, mYaw);

		// now all is said and done, calc relative vectors
		mU = tr - tl;
		mV = bl - tl;

		// transform top left to world
		mTopLeft = tl + mOrigin;

		sanityCheck();
	}

	void buildLookVectors()
	{
		mLookForward = glm::rotateX(glm::vec3(0, 0, 1), mPitch);
		mLookForward = glm::rotateY(mLookForward, mYaw);

		mLookRight = glm::rotateY(glm::vec3(1, 0, 0), mYaw);
	}

	// safe delta functions - from user input
	void moveFov(float d)
	{
		mDirty = true;
		mFov = glm::clamp(mFov + d, (float)EPSILON, PI - EPSILON);
	}

	// move forwards/backwards (positive = forwards)
	// TODO could optimise this - use a bool instead of multiplying by d
	// we only ever move either fwd or back (not varying distances)
	void moveForward(float d)
	{
		mDirty = true;
		mOrigin += d * mLookForward;
	}

	// move left/right (positive = right)
	void moveRight(float d)
	{
		mDirty = true;
		mOrigin += d * mLookRight;
	}

	void moveYawPitch(float deltaYaw, float deltaPitch)
	{
		mDirty = true;

		mYaw = fmod(mYaw + deltaYaw, PI * 2);

		// pitch is limited to look directly up / down (ie no upside down)
		mPitch = clamp(mPitch + deltaPitch, -PI / 2, PI / 2);

		buildLookVectors();
	}

	int width() const { return mWidth; }
	int height() const { return mHeight; }

	float yaw() const { return mYaw; }
	float pitch() const { return mPitch; }
	float fov() const { return mFov; }

	// screen point + u & v vectors 
	const glm::vec3& topLeft() const { return mTopLeft; }
	const glm::vec3& u() const { return mU; }
	const glm::vec3& v() const { return mV; }

	// origin (eye point)
	const glm::vec3& origin() const { return mOrigin; }

	// note - these 3 don't call buildCamera() 
	void setYaw(float yaw) { mYaw = yaw; }
	void setPitch(float pitch) { mPitch = pitch; }
	void setFov(float fov) { mFov = fov; }

private:
	// screen res in pixels
	int mWidth, mHeight;

	// These are the render-time params - ie calculated once per frame(max) to save work
	// origin position in world space
	glm::vec3 mOrigin;

	// point at top left of screen in world space
	glm::vec3 mTopLeft;

	// u (horiz) and v (vertical) vectors from top_left point
	glm::vec3 mU, mV;

	// direction we're facing - build from yaw/pitch, used by moveForward/MoveRight()
	glm::vec3 mLookForward, mLookRight;

	// These are the input-params - ie changed by user input, and rendered into the params 
	// above by buildCamera()
	float mYaw, mPitch, mFov;

	// these are the starting params - optionally loaded from the scenefile
	// pressing 'r' at runtime will return to these values
	glm::vec3 mStartingOrigin;
	float mStartingPitch, mStartingYaw, mStartingFov;

	bool mDirty;

	inline bool feq(float a, float b)
	{
		return a < b + EPSILON || a < b - EPSILON;
	}

	// stupid lack of c++17
	template<class T>
	T clamp(T val, T lo, T hi)
	{
		return min(max(val, lo), hi);
	}

	// check an angle is clamped -2pi < angle < 2pi (ie within one rotation either way)
	inline bool isAngleInOneRev(float angle)
	{
		return angle > -2 * PI  && angle < 2 * PI;
	}

	inline bool isAngleInHalfRev(float angle)
	{
		return angle >= 0 && angle < PI;
	}

	inline unsigned int largestElem(glm::vec3 const& v)
	{
		return v.x > v.y ?
			(v.x > v.z ? 0 : 2) :
			(v.y > v.z ? 1 : 2);
	}

	// return a very shot vector (a smidge) 
	inline glm::vec3 smidge(const glm::vec3& v)
	{
		return v * 1e-4f; //const float SMIDGE_LEN = 1e-4f;
	}
};



//#pragma once
//#include "template.h"
//
//class Camera
//{
//public:
//	//check glm documentation for rotation of camera.
//	mat4 transformMatrix; // your transformation matrix.
//	vec3 position;
//	vec3 wUp;
//	vec3 rUp;
//	vec3 rRight;
//
//	vec3 viewDirection;
//	vec3 screenCenter;
//	vec3 p0, p1, p2;
//
//	float d;
//	float width;
//	float height;
//	float ratio;
//	
//	Ray* primaryRays[SCRWIDTH*SCRHEIGHT];
//
//	Camera();
//	void Init();
//	void GenerateRays();
//	void UpdateRays();
//	void TransformCamera(mat4 transformMatrix);
//	void UpdatePosition();
//};
