#pragma once
#include <glm/glm.hpp>

namespace kaczka
{
	class WaterSimulation {
	public:
		static constexpr int N = 256;
	private:
		static constexpr float c = 1.0f;
		static constexpr float A = 0.25f * c * c * (N - 1) * (N - 1) / (N * N), B = 2.0f - 4.0f * A;

		float(*d)[N], (*tab)[N], (*prevTab)[N];
	public:
		glm::vec4* normals;

		WaterSimulation()
		{
			tab = new float[N][N];
			prevTab = new float[N][N];
			d = new float[N][N];
			normals = new glm::vec4[N * N];

			for (int i = 0; i < N; ++i)
				for (int j = 0; j < N; ++j)
				{
					d[i][j] = 0.95f * fminf(1.0f, 10.0f * fminf(1.0f - abs((2.0f * j - (N-1)) / (N-1)), 1.0f - abs((2.0f * i - (N-1)) / (N-1))));
					tab[i][j] = 0.0f;
					prevTab[i][j] = 0.0f;
					normals[i * N + j] = { 0.5f,0.5f,1.0f,1.0f };
				}
		}

		~WaterSimulation()
		{
			delete[] tab;
			delete[] prevTab;
			delete[] d;
			delete[] normals;
		}

		void disturb(int i, int j)
		{
			tab[i][j] += 0.25f;
		}

		void propagate()
		{
			int i = 0, j = 0;
			prevTab[i][j] = d[i][j] * (A * (tab[i + 1][j] + tab[i][j + 1]) + B * tab[i][j] - prevTab[i][j]);
			for (int j = 1; j < N - 1; ++j)
			{
				prevTab[i][j] = d[i][j] * (A * (tab[i + 1][j] + tab[i][j - 1] + tab[i][j + 1]) + B * tab[i][j] - prevTab[i][j]);
			}
			prevTab[i][j] = d[i][j] * (A * (tab[i + 1][j] + tab[i][j - 1]) + B * (tab[i][j]) - prevTab[i][j]);
			for (i = 1; i < N - 1; ++i)
			{
				j = 0;
				prevTab[i][j] = d[i][j] * (A * (tab[i - 1][j] + tab[i + 1][j] + tab[i][j + 1]) + B * tab[i][j] - prevTab[i][j]);
				for (j = 1; j < N - 1; ++j)
				{
					prevTab[i][j] = d[i][j] * (A * (tab[i - 1][j] + tab[i + 1][j] + tab[i][j - 1] + tab[i][j + 1]) + B * tab[i][j] - prevTab[i][j]);
				}
				prevTab[i][j] = d[i][j] * (A * (tab[i - 1][j] + tab[i + 1][j] + tab[i][j - 1]) + B * tab[i][j] - prevTab[i][j]);
			}
			j = 0;
			prevTab[i][j] = d[i][j] * (A * (tab[i - 1][j] + tab[i][j + 1]) + B * tab[i][j] - prevTab[i][j]);
			for (j = 1; j < N - 1; ++j)
			{
				prevTab[i][j] = d[i][j] * (A * (tab[i - 1][j] + tab[i][j - 1] + tab[i][j + 1]) + B * tab[i][j] - prevTab[i][j]);
			}
			prevTab[i][j] = d[i][j] * (A * (tab[i - 1][j] + tab[i][j - 1]) + B * tab[i][j] - prevTab[i][j]);

			float(*tmp)[N] = tab;
			tab = prevTab;
			prevTab = tmp;
		}

		void computeNormals()
		{
			for (int i = 1; i < N - 1; ++i)
			{
				for (int j = 1; j < N - 1; ++j)
				{
					glm::vec3 dx = { 2.0f / N,0.0f,(tab[i + 1][j] - tab[i - 1][j]) * 0.5f };
					glm::vec3 dy = { 0.0f,2.0f / N,(tab[i][j + 1] - tab[i][j - 1]) * 0.5f };
					normals[i * N + j] = glm::vec4(0.5f * glm::normalize(glm::cross(dx, dy)) + 0.5f, 1.0f);
				}
			}
		}
	};
}