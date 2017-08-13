//
// Created by Tiago on 09-Feb-17.
//

#include "Renderer.hpp"

using MobileRT::Renderer;

Renderer::Renderer (std::unique_ptr<Shader> &&shader,
                    std::unique_ptr<Camera> &&camera,
                    std::unique_ptr<Sampler> &&samplerPixel,
                    const unsigned width, const unsigned height,
                    const unsigned samplesPixel) noexcept :
  camera_ {camera.release ()},
  shader_ {shader.release ()},
  samplerPixel_ {samplerPixel.release ()},
  accumulate_ {std::vector<RGB> {width * height}},
  blockSizeX_ {width / static_cast<unsigned>(std::sqrt (NumberOfBlocks))},
  blockSizeY_ {height / static_cast<unsigned>(std::sqrt (NumberOfBlocks))},
  sample_ {0},
  width_ {width},
  height_ {height},
  domainSize_ {(width / blockSizeX_) * (height / blockSizeY_)},
  resolution_ {width * height},
  samplesPixel_ {samplesPixel}
{
}

void Renderer::renderFrame (unsigned *const bitmap, const int numThreads) noexcept {
	LOG("START - resolution = ", resolution_);
	this->sample_ = 0;
	for (auto accumulate : this->accumulate_) {
		accumulate.reset();
	}
	this->samplerPixel_->resetSampling();
	this->shader_->resetSampling();
  const int numChildren {numThreads - 1};
  std::vector<std::thread> threads {};
	threads.reserve(static_cast<unsigned>(numChildren));
  for (int i {0}; i < numChildren; i ++) {
		threads.emplace_back(&Renderer::renderScene, this, bitmap, i);
	}
	renderScene(bitmap, numChildren);
	for (std::thread& thread : threads) {
		thread.join();
	}
	threads.clear();

  float max {- 1.0f};
  unsigned ii {0};
  unsigned jj {0};
  for (unsigned i {0}; i < this->height_; i++) {
    for (unsigned j {0}; j < this->width_; j++) {
      const unsigned pixel {i * this->width_ + j};
      const float pixelMax {this -> accumulate_[pixel] . getMax ()};
			if (pixelMax > max) {
				max = pixelMax;
				ii = i;
				jj = j;
			}
		}
	}
	LOG("max = ", max, ", i = ", ii, ", j = ", jj);
	LOG("r = ", this->accumulate_[ii*this->width_ + jj].R_, ", g = ", this->accumulate_[ii*this->width_ + jj].G_, ", b = ", this->accumulate_[ii*this->width_ + jj].B_, " , s = ", this->accumulate_[ii*this->width_ + jj].samples_, " , color = ", this->accumulate_[ii*this->width_ + jj].getColor());

	LOG("point3D = ", Point3D::getInstances());
	LOG("vector3D = ", Vector3D::getInstances());
	LOG("RGB = ", RGB::getInstances());
	LOG("scene = ", Scene::getInstances());
	LOG("ray = ", Ray::getInstances());
	LOG("material = ", Material::getInstances());
	LOG("intersection = ", Intersection::getInstances());
	LOG("FINISH");
}

void Renderer::stopRender() noexcept {
	this->blockSizeX_ = 0;
	this->blockSizeY_ = 0;
  this->samplerPixel_->stopSampling ();
}

void Renderer::renderScene (unsigned *const bitmap, const int tid) noexcept {
  const float INV_IMG_WIDTH {1.0f / this->width_};
  const float INV_IMG_HEIGHT {1.0f / this->height_};
  const float pixelWidth {0.5f / this->width_};
  const float pixelHeight {0.5f / this->height_};
  const unsigned samples {this->samplesPixel_};
  RGB pixelRGB {};
  Intersection intersection {};
  for (unsigned sample {0}; sample < samples; sample++) {
		while (true) {
      const float block {this->camera_->getBlock (sample)};
			if (block >= 1.0f) {break;}
      const unsigned pixel {
        static_cast<unsigned>(static_cast<uint32_t>(roundf (block * this->domainSize_)) *
                                  this -> blockSizeX_ % resolution_)};
      const unsigned startY {
        ((pixel / this -> width_) * this -> blockSizeY_) % this -> height_};
      const unsigned endY {startY + this->blockSizeY_};
      for (unsigned y {startY}; y < endY; y++) {
        const unsigned yWidth {y * this->width_};
        const float v {y * INV_IMG_HEIGHT};
        const unsigned startX {(pixel + yWidth) % this->width_};
        const unsigned endX {startX + this->blockSizeX_};
        for (unsigned x {startX}; x < endX; x++) {
          const float u {x * INV_IMG_WIDTH};
          const float r1 {this->samplerPixel_->getSample ()};
          const float r2 {this->samplerPixel_->getSample ()};
          const float deviationU {(r1 - 0.5f) * 2.0f * pixelWidth};
          const float deviationV {(r2 - 0.5f) * 2.0f * pixelHeight};
          Ray ray {this -> camera_ -> generateRay (u, v, deviationU, deviationV)};
					pixelRGB.reset(); //pixel color without intersection
          intersection . length_ = RayLengthMax;
					intersection.material_ = nullptr;
					// LOG("triangles = ", shader_.scene_.triangles_.size());
					// LOG("spheres = ", shader_.scene_.spheres_.size());
					// LOG("planes = ", shader_.scene_.planes_.size());
					this->shader_->rayTrace(&pixelRGB, &intersection, std::move(ray));
					this->accumulate_[yWidth + x].addSampleAndCalcAvg(&pixelRGB);
					bitmap[yWidth + x] = pixelRGB.RGB2Color();
				}
			}
		}
		if (tid == 0) {
			this->sample_ = sample + 1;
			LOG("Samples terminados = ", this->sample_);
		}
	}
}

unsigned Renderer::getSample () const noexcept {
    return this->sample_;
}
