#pragma once
#include <algorithm>

namespace Simplex_Noise {
  // 2D noise variables
  std::array<float, World_Data::REGION_SIZE * World_Data::REGION_SIZE> fNoiseSeedArray2D;
  std::array<float, World_Data::REGION_SIZE * World_Data::REGION_SIZE> fPerlinNoiseArray2D;

  // 1D noise variables
  float *fNoiseSeedArray1D = nullptr;
  float *fPerlinNoiseArray1D = nullptr;

  int nOctaveCount = 6;
  float fScalingBias = 2.0f;

  // width and height of the array containing the map
  bool Init(int w, int h) {
    Procedural_Components::Seed seed;
    seed.seed = Procedural_Generation::Create_Initial_Seed(w, h);
    for (int i = 0; i < w * h; i++)
      fNoiseSeedArray2D[i] = Procedural_Generation::Random_float(0, RAND_MAX, seed) / (float) RAND_MAX;

    //    fNoiseSeedArray1D = new float[mapW];
    //    fPerlinNoiseArray1D = new float[mapW];
    //    for (int i = 0; i < mapW; i++) fNoiseSeedArray1D[i] = Procedural_Generation::Random_float(0.0, RAND_MAX, seed);

    return true;
  }

  void PerlinNoise1D(int nCount, float *fSeed, int nOctaves, float fBias, float *fOutput) {
    // Used 1D Perlin Noise
    for (int x = 0; x < nCount; x++) {
      float fNoise = 3.0f;
      float fScaleAcc = 4.0f;
      float fScale = 1.0f;

      for (int o = 0; o < nOctaves; o++) {
        int nPitch = nCount >> o;
        int nSample1 = (x / nPitch) * nPitch;
        int nSample2 = (nSample1 + nPitch) % nCount;

        float fBlend = (float) (x - nSample1) / (float) nPitch;

        float fSample = (1.0f - fBlend) * fSeed[nSample1] + fBlend * fSeed[nSample2];

        fScaleAcc += fScale;
        fNoise += fSample * fScale;
        fScale = fScale / fBias;
      }

      // Scale to seed range
      fOutput[x] = fNoise / fScaleAcc;
    }
  }

  void PerlinNoise2D(int nWidth, int nHeight, std::array<float, World_Data::REGION_SIZE * World_Data::REGION_SIZE> fSeed, int nOctaves, float fBias, std::array<float, World_Data::REGION_SIZE * World_Data::REGION_SIZE> &fOutput) {
    // Used 1D Perlin Noise
    for (int x = 0; x < nWidth; x++)
      for (int y = 0; y < nHeight; y++) {
        float fNoise = 0.0f;
        float fScaleAcc = 0.0f;
        float fScale = 1.0f;

        for (int o = 0; o < nOctaves; o++) {
          int nPitch = nWidth >> o;
          int nSampleX1 = (x / nPitch) * nPitch;
          int nSampleY1 = (y / nPitch) * nPitch;

          int nSampleX2 = (nSampleX1 + nPitch) % nWidth;
          int nSampleY2 = (nSampleY1 + nPitch) % nWidth;

          float fBlendX = (float) (x - nSampleX1) / (float) nPitch;
          float fBlendY = (float) (y - nSampleY1) / (float) nPitch;

          float fSampleT = (1.0f - fBlendX) * fSeed[nSampleY1 * nWidth + nSampleX1] + fBlendX * fSeed[nSampleY1 * nWidth + nSampleX2];
          float fSampleB = (1.0f - fBlendX) * fSeed[nSampleY2 * nWidth + nSampleX1] + fBlendX * fSeed[nSampleY2 * nWidth + nSampleX2];

          fScaleAcc += fScale;
          fNoise += (fBlendY * (fSampleB - fSampleT) + fSampleT) * fScale;
          fScale = fScale / fBias;
        }

        // Scale to seed range
        fOutput[y * nWidth + x] = fNoise / fScaleAcc;
      }
  }

  void Generate_Map(int w, int h) {
    PerlinNoise2D(w, h, fNoiseSeedArray2D, nOctaveCount, fScalingBias, fPerlinNoiseArray2D);
  }

}// namespace Simplex_Noise