#pragma once

#include <cstdint>
#include <vector>
#include "random.hpp"
using Random = effolkronium::random_static;

template <typename T>
concept HasTargetFunction = requires(T t) { { t.GetTargetValue() } -> std::convertible_to<float>; };

template <typename T>
concept HasCrossoverFunction = requires(T f, const T& s) { { f.Crossover(s) } -> std::convertible_to<T>; };

template <typename T>
concept HasMutationFunction = requires(T t) { { t.Mutation() }; };

template <typename T>
concept ValidBreed = HasTargetFunction<T>;//&& HasCrossoverFunction<T>&& HasMutationFunction<T>;



template <ValidBreed Breed>
static auto MakeStep(std::vector<Breed>& current_generation) -> std::vector<Breed>
{
	std::vector<Breed> parents;
	std::vector<Breed> new_generation;

	/*
		Step 1 - Calculate target values
	*/
	std::vector<float> fitnes_values(current_generation.size(), 0.0f);
	std::vector<float> fitnes_coeffs(current_generation.size(), 0.0f);
	float fitnes_total = 0.0f;
	float fitnes_avg = 0.0f;
	float min = 0;

	// Calculate target values and total
	for (int i = 0; i < current_generation.size(); i++)
	{
		fitnes_values[i] = current_generation[i].GetTargetValue();
        min = std::min(fitnes_values[i], min);
	}

	for (int i = 0; i < current_generation.size(); i++)
	{
		fitnes_values[i] -= min;
		fitnes_total += fitnes_values[i];
	}
    fitnes_avg = fitnes_total / current_generation.size();

	// Calculate relative target coefficients
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
			fitnes_coeffs[i] -= 1.0f;
			parents.push_back(current_generation[i]);
		}
	}
	
	// Fill the rest places randomly
	while (parents.size() < current_generation.size())
	{
		for (int i = 0; i < current_generation.size(); i++)
		{
			if (fitnes_values[i] >= Random::get(0.0f, fitnes_avg))
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
		int first_parent_id  = Random::get<uint32_t>(0, current_generation.size() - 1);
		int second_parent_id = Random::get<uint32_t>(0, current_generation.size() - 1);
		while (first_parent_id == second_parent_id)
		{
			second_parent_id = Random::get<uint32_t>(0, current_generation.size() - 1);
		}
		Breed child = parents[first_parent_id];
		child = parents[first_parent_id].Crossover(parents[second_parent_id]);
		child.Mutation();
		new_generation.push_back(child);
	}
	return new_generation;
}


template <ValidBreed Breed>
auto GeneticAlgorithm(std::vector<Breed> population, int steps) -> std::vector<std::vector<Breed>>
{
	std::vector<std::vector<Breed>> trace;

	for (int generation = 0; generation < steps; generation++)
	{
		LOG_INFO("Iteration: {}", generation);
		trace.push_back(MakeStep(population));
	}
	return trace;
}