/*
* Script which walks through a simple Lorenz attractor implementation and solver
*
* Created on: Feb. 11, 2020
* Author: Victoria Preston
*/

#include <iostream>
#include <fstream>
#include <iterator>
#include <vector>
#include <functional>
#include <algorithm>

typedef std::vector<double> state;

const double sigma = 10.0;
const double R = 28.0;
const double b = 8.0 / 3.0;

// define the lorenz function
void lorenz(state& x, state& dxdt, double t)
{
	dxdt[0] = sigma * (x[1] - x[0]);
	dxdt[1] = R * x[0] - x[1] - x[0] * x[2];
	dxdt[2] = x[0] * x[1] - b * x[2];
}

void euler(std::function<void(state&, state&, double)> func, state x, double h, double t, int T)
{
	state m( 3 );
	std::cout << "Starting state is " << t << std::endl;
	std::ofstream logger;
	logger.open("/home/vpreston/Documents/FUMES/model_ws/src/phenom_simulator/logs/test.txt");
	std::ostream_iterator<double> output_iterator(logger, ",");
    std::copy(x.begin(), x.end(), output_iterator);
	logger << t << std::endl;
	for (int i = 0; i < T; ++i)
	{
		func(x, m, i);
		state temp( 3 );
		std::transform(m.begin(), m.end(), temp.begin(),
			           std::bind(std::multiplies<double>(), std::placeholders::_1, h)); 
		std::transform(x.begin(), x.end(), temp.begin(), x.begin(), std::plus<double>());
		t = t + h;
		std::cout << "Current x value is " << t << std::endl;
		std::copy(x.begin(), x.end(), output_iterator);
		logger << t << std::endl;
	}
	logger.close();
}

int main()
{
	const double dt = 0.01;
	state x1( 3 );
	x1[0] = 1.0;
	x1[1] = 0.0;
	x1[2] = 0.0;

	euler(lorenz, x1, dt, 0, 100000);

	return 0;
}