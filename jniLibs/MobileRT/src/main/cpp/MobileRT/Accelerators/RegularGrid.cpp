//
// Created by Tiago on 6-Mar-17.
//

#include "RegularGrid.hpp"

using MobileRT::RegularGrid;

RegularGrid::RegularGrid(Point3D min, Point3D max, Scene *scene, const int gridSize, const int gridShift) :
	spheres_(std::vector<std::vector<Sphere*>> (static_cast<size_t> (gridSize * gridSize * gridSize))),
	//scene_(scene),
	gridSize_(gridSize),
	gridShift_(gridShift),
	m_Extends (AABB( min, max ))
{
	for (unsigned int i (0); i < static_cast<unsigned int> (gridSize_ * gridSize_ * gridSize_); i++) {
		spheres_[i] = std::vector<Sphere*> ();
	}

	// precalculate 1 / size of a cell (for x, y and z)
	m_SR.x_ = gridSize_ / (m_Extends.pointMax_.x_ - m_Extends.pointMin_.x_);
	m_SR.y_ = gridSize_ / (m_Extends.pointMax_.y_ - m_Extends.pointMin_.y_);
	m_SR.z_ = gridSize_ / (m_Extends.pointMax_.z_ - m_Extends.pointMin_.z_);
	// precalculate size of a cell (for x, y, and z)
	m_CW = (m_Extends.pointMax_ -  m_Extends.pointMin_) * (1.0f / gridSize_);

	const float dx ((max.x_ - min.x_) / gridSize_);
	const float dy ((max.y_ - min.y_) / gridSize_);
	const float dz ((max.z_ - min.z_) / gridSize_);

	const float dx_reci (1.0f / dx);
	const float dy_reci (1.0f / dy);
	const float dz_reci (1.0f / dz);

	size_t spheresSize (scene->spheres_.size());
	// store primitives in the grid cells
	for ( size_t p (0); p < spheresSize; p++ ) {
		Sphere &sphere(scene->spheres_[p]);
		AABB bound (sphere.getAABB());
		Point3D bv1 (bound.pointMin_);
		Point3D bv2 (bound.pointMax_);

		// find out which cells could contain the primitive (based on aabb)
		auto x1 (static_cast<int>((bv1.x_ - min.x_) * dx_reci));
		auto x2 (static_cast<int>((bv2.x_ - min.x_) * dx_reci) + 1);
		x1 = (x1 < 0) ? 0 : x1;
		x2 = (x2 > (gridSize_ - 1)) ? gridSize_ - 1 : x2;
		auto y1 (static_cast<int>((bv1.y_ - min.y_) * dy_reci));
		auto y2 (static_cast<int>((bv2.y_ - min.y_) * dy_reci) + 1);
		y1 = (y1 < 0) ? 0 : y1;
		y2 = (y2 > (gridSize_ - 1)) ? gridSize_ - 1 : y2;
		auto z1 (static_cast<int>((bv1.z_ - min.z_) * dz_reci));
		auto z2 (static_cast<int>((bv2.z_ - min.z_) * dz_reci) + 1);
		z1 = (z1 < 0) ? 0 : z1;
		z2 = (z2 > (gridSize_ - 1)) ? gridSize_ - 1 : z2;

		for ( int x (x1); x < x2; x++ ) {
			for ( int y (y1); y < y2; y++ ) {
				for ( int z (z1); z < z2; z++ ) {
					// construct aabb for current cell
					const auto idx (static_cast<size_t>(x) + static_cast<size_t>(y) * static_cast<size_t>(gridSize_) + static_cast<size_t>(z) * static_cast<size_t>(gridSize_) * static_cast<size_t>(gridSize_));
					const Point3D pos( min.x_ + x * dx, min.y_ + y * dy, min.z_ + z * dz );
					const AABB cell( pos, Point3D( dx, dy, dz ) );
					// do an accurate aabb / primitive intersection test
					if (sphere.intersect( cell )) {
						spheres_[idx].emplace_back(&sphere);
					}
				}
			}
		}
	}

}

bool RegularGrid::intersect(Intersection *intersection, const Ray &ray) const noexcept {
	bool retval (false);
	Vector3D const &raydir(ray.direction_);
	Point3D const &curpos(ray.origin_);
	AABB e (m_Extends);
	// setup 3DDDA (double check reusability of primary ray data)
	Vector3D cb, tmax, tdelta;
	Vector3D cell ((curpos - e.pointMin_) * m_SR);
	int stepX, outX, X = static_cast<int>(cell.x_);
	int stepY, outY, Y = static_cast<int>(cell.y_);
	int stepZ, outZ, Z = static_cast<int>(cell.z_);

	if ((X < 0) || (X >= gridSize_) || (Y < 0) || (Y >= gridSize_) || (Z < 0) || (Z >= gridSize_)) {
		return false;
	}

	if (raydir.x_ > 0) {
		stepX = 1;
		outX = gridSize_;
		cb.x_ = e.pointMin_.x_ + (X + 1) * m_CW.x_;
	} else {
		stepX = -1;
		outX = -1;
		cb.x_ = e.pointMin_.x_ + X * m_CW.x_;
	}

	if (raydir.y_ > 0.0f) {
		stepY = 1;
		outY = gridSize_;
		cb.y_ = e.pointMin_.y_ + (Y + 1) * m_CW.y_; 
	} else {
		stepY = -1;
		outY = -1;
		cb.y_ = e.pointMin_.y_ + Y * m_CW.y_;
	}

	if (raydir.z_ > 0.0f) {
		stepZ = 1;
		outZ = gridSize_;
		cb.z_ = e.pointMin_.z_ + (Z + 1) * m_CW.z_;
	} else {
		stepZ = -1;
		outZ = -1;
		cb.z_ = e.pointMin_.z_ + Z * m_CW.z_;
	}

  if (raydir.x_ != 0.0f) {
		const float rxr (1.0f / raydir.x_);
		tmax.x_ = (cb.x_ - curpos.x_) * rxr; 
		tdelta.x_ = m_CW.x_ * stepX * rxr;
	} else {
		tmax.x_ = 1000000;
	}

  if (raydir.y_ != 0.0f) {
		const float ryr (1.0f / raydir.y_);
		tmax.y_ = (cb.y_ - curpos.y_) * ryr; 
		tdelta.y_ = m_CW.y_ * stepY * ryr;
	} else {
		tmax.y_ = 1000000;
	}

  if (raydir.z_ != 0.0f) {
		const float rzr (1.0f / raydir.z_);
		tmax.z_ = (cb.z_ - curpos.z_) * rzr; 
		tdelta.z_ = m_CW.z_ * stepZ * rzr;
	} else {
		tmax.z_ = 1000000;
	}

	// start stepping
	Sphere *const *list (nullptr);
	// trace primary ray
	while (true) {
		list = spheres_[static_cast<size_t>(X) + (static_cast<size_t>(Y) << gridShift_) + (static_cast<size_t>(Z) << (static_cast<size_t>(gridShift_) * 2))].data();

		size_t i (0);
		size_t spheresSize (spheres_[static_cast<size_t>(X) + (static_cast<size_t>(Y) << static_cast<size_t>(gridShift_)) + (static_cast<size_t>(Z) << (static_cast<size_t>(gridShift_) * 2))].size());
		while (i < spheresSize) {
			Sphere* pr (list[i]);
			bool result (false);
			// if (pr->GetLastRayID() != a_Ray.GetID()) {
				result = pr->intersect( intersection, ray );
				if (result) {
					retval = result;
					goto testloop;
				}
			// }
			i++;
		}

		if (tmax.x_ < tmax.y_) {
			if (tmax.x_ < tmax.z_) {
				X = X + stepX;
				if (X == outX) {
					return false;
				}
				tmax.x_ += tdelta.x_;
			} else {
				Z = Z + stepZ;
				if (Z == outZ) {
					return false;
				}
				tmax.z_ += tdelta.z_;
			}
		}
		else {
			if (tmax.y_ < tmax.z_) {
				Y = Y + stepY;
				if (Y == outY) {
					return false;
				}
				tmax.y_ += tdelta.y_;
			} else {
				Z = Z + stepZ;
				if (Z == outZ) {
					return false;
				}
				tmax.z_ += tdelta.z_;
			}
		}

	}

	testloop:
	while (true) {
		list = spheres_[static_cast<size_t>(X) + (static_cast<size_t>(Y) << static_cast<size_t>(gridShift_)) + (static_cast<size_t>(Z) << (static_cast<size_t>(gridShift_) * 2))].data();

		size_t i (0);
		size_t spheresSize (spheres_[static_cast<size_t>(X) + (static_cast<size_t>(Y) << static_cast<size_t>(gridShift_)) + (static_cast<size_t>(Z) << (static_cast<size_t>(gridShift_) * 2))].size());
		while (i < spheresSize) {
			Sphere* pr (list[i]);
			bool result (false);
			// if (pr->GetLastRayID() != a_Ray.GetID()) {
				result = pr->intersect( intersection, ray );
				if (result) {
					retval = result;
				}
			// }
			i++;
		}
		if (tmax.x_ < tmax.y_) {
			if (tmax.x_ < tmax.z_) {
				if (intersection->length_ < tmax.x_) {
					break;
				}
				X = X + stepX;
				if (X == outX) {
					break;
				}
				tmax.x_ += tdelta.x_;
			}	else {
				if (intersection->length_ < tmax.z_) {
					break;
				}
				Z = Z + stepZ;
				if (Z == outZ) {
					break;
				}
				tmax.z_ += tdelta.z_;
			}
		} else {
			if (tmax.y_ < tmax.z_) {
				if (intersection->length_ < tmax.y_) {
					break;
				}
				Y = Y + stepY;
				if (Y == outY) {
					break;
				}
				tmax.y_ += tdelta.y_;
			} else {
				if (intersection->length_ < tmax.z_) {
					break;
				}
				Z = Z + stepZ;
				if (Z == outZ) {
					break;
				}
				tmax.z_ += tdelta.z_;
			}
		}
	}

	return retval;
}
