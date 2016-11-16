//
// Created by Tiago on 16-10-2016.
//

#include "ShaderWhitted.h"
#include "../Constants.h"

using namespace MobileRT;

ShaderWhitted::ShaderWhitted(RayTracer& rayTracer, const Scene& scene) :
		Shader(rayTracer, scene)
{
}

void ShaderWhitted::shade(RGB& rgb, Intersection& intersection, const Ray& ray, Vector3D& vectIntersectCamera) const
{
	// the normal always points to outside objects (e.g., spheres)
	// if the cosine between the ray and the normal is less than 0 then
	// the ray intersected the object from the inside and the shading normal
	// should be symmetric to the geometric normal
	Vector3D& shadingNormal ((ray.direction_.dotProduct(intersection.normal_) < 0.0f)?
						  intersection.normal_// entering the object
						: intersection.getSymNormal());// We have to reverse the normal now

	rgb.recycle();
	// shadowed direct lighting - only for diffuse materials
	if (intersection.material_->Kd_.isZero() == false)
	{
		const unsigned int sizeLights (scene_.lights.size());
		Intersection intersectLight;

		for (unsigned int i (0); i < sizeLights ; i++)//para cada luz
		{
			const PointLight* light (scene_.lights[i]);

			vectIntersectCamera.recycle(light->position_, intersection.point_);//calcula vetor desde a interseçao até à luz
			const float distanceToLight (vectIntersectCamera.normalize());//distancia da interseçao à luz (e normaliza-o)
			const float cos_N_L (vectIntersectCamera.dotProduct(shadingNormal));//x*x + y*y + z*z
			if (cos_N_L > 0.0f)
			{
				//raio de sombra - orig=interseçao, dir=luz
				const Ray shadowRay(intersection.point_, vectIntersectCamera, distanceToLight, ray.depth_ + 1);
				//intersectLight = ();//interseçao do raio de sombra com a primitiva mais proxima
				if (scene_.shadowTrace(intersectLight, shadowRay) == false)//se nao ha nenhuma primitiva entre a interseçao e a luz
				{
					RGB diffuseRad(light->radiance_);//R=1, G=1, B=1
					diffuseRad.mult(intersection.material_->Kd_);//cor da luz
					diffuseRad.mult (cos_N_L);//angulo em relaçao a normal
					rgb.add(diffuseRad);//adiciona a cor da luz
				}
			}
		}
		// ambient light
		rgb.R_ += intersection.material_->Kd_.R_ * 0.1f;
		rgb.G_ += intersection.material_->Kd_.G_ * 0.1f;
		rgb.B_ += intersection.material_->Kd_.B_ * 0.1f;
	} // end direct + ambient
	// specular reflection
	if ((intersection.material_->Ks_.isZero() == false) && (ray.depth_ < this->MAX_DEPTH))
	{
		// compute specular reflection
		//raio de reflexao
		const float RN_dot (2.0f * shadingNormal.dotProduct(ray.symDirection_));
		shadingNormal.mult (RN_dot);
		shadingNormal.sub (ray.symDirection_);
		shadingNormal.normalize();

		Ray specRay(intersection.point_, shadingNormal, RAY_LENGTH_MAX, ray.depth_ + 1);
		RGB specRad;
		Intersection isec;
		this->rayTracer_.rayTrace(specRad, specRay, isec, vectIntersectCamera);
		specRad.mult(intersection.material_->Ks_);
		rgb.add(specRad);
	}
}