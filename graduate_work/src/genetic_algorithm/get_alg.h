#pragma once

#include <cstdint>
#include <vector>
#include <random>


static inline uint32_t GrayEncode(unsigned int g)
{
	return g ^ (g >> 1);
}

static inline uint32_t GrayDecode(unsigned int gray)
{
	uint32_t bin = 0;
	while (gray != 0)
	{
		gray >>= 1;
		bin ^= gray;
	}
	return bin;
}


template <typename T>
concept HasTargetFunction = requires(const T & arg) { { T::GetTargetValue(arg) } -> std::convertible_to<float>; };

template <typename T>
concept HasCrossoverFunction = requires(const T & f, const T & s) { { T::Crossover(f, s) } -> std::convertible_to<T>; };

template <typename T>
concept ValidBread = HasTargetFunction<T> && HasCrossoverFunction<T>;


template <ValidBread Bread>
static auto MakeStep(std::vector<Bread>& current_generation) -> bool
{
	std::vector<Bread> parents;
	std::vector<Bread> new_generation;

	/*
		Step 1 - Calculate target values
	*/
	// Calculate target values
	std::vector<float> fitnes_values(0.0f, current_generation.size());
	std::vector<float> fitnes_coeffs(0.0f, current_generation.size());
	float fitnes_total = 0.0f;
	float fitnes_avg = 0.0f;

	// Calculate target values and total
	for (int i = 0; i < current_generation.size(); i++)
	{
		fitnes_values[i] = Bread::fitnes(current_generation[i]);
		fitnes_total += fitnes_values[i];
	}
	fitnes_avg = fitnes_total / current_generation.size();
	
	// Calculate target coefficients
	for (int i = 0; i < current_generation.size(); i++)
	{
		fitnes_coeffs[i] = fitnes_values[i] / fitnes_avg;
	}
	
	/*
		Step 2 - Choose parents
	*/
	// Take strongest parents
	for (int i = 0; i < current_generation.size(); i++)
	{
		while (fitnes_coeffs[i] > 1.0f
			&& parents.size() < current_generation.size())
		{
			fitnes_coeffs[i]--;
			parents.push_back(current_generation[i]);
		}
	}
	
	// Fill the rest gaps randomly
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<> rgn(0.0f, fitnes_total);

	while (parents.size() < current_generation.size())
	{
		for (int i = 0; i < current_generation.size(); i++)
		{
			if (fitnes_values[i] >= rgn(gen))
			{
				parents.push_back(current_generation[i]);
			}
		}
	}
	
	/*
		Step 3 - Create new generation
	*/
	// Move beast entity to the next generation (Helps to avoid degradation)
	int index = 0;
	float max = fitnes_values[0];

	for (int i = 1; i < current_generation.size(); i++)
	{
		if (max < fitnes_values[i])
		{
			index = i;
			max = fitnes_values[i];
		}
	}
	new_generation.push_back(current_generation[index]);

	// Make child from two parents
	for (int i = 0; i < current_generation.size() - 1; i++)
	{
		int first_parent_id  = rand() % current_generation.size();
		int second_parent_id = rand() % current_generation.size();
		while (first_parent_id != second_parent_id)
		{
			second_parent_id = rand() % current_generation.size();
		}
		new_generation.push_back(Bread::Crossover(parents[first_parent_id], parents[second_parent_id]));
	}

	std::swap(current_generation, new_generation);
	return true;
}



template <ValidBread Bread>
auto GeneticAlgorithm(std::vector<Bread> population, int steps) -> std::vector<Bread>
{
	int generation = 0;

	while (generation < steps)
	{
		if (MakeStep(population))
		{
			break;
		}
	}

	return std::move(population);
}