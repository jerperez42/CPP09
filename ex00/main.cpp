/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jerperez <jerperez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/03 10:41:02 by jerperez          #+#    #+#             */
/*   Updated: 2024/05/03 14:50:52 by jerperez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <string>
#include "BitcoinExchange.hpp"

#define BE_DBRATES "data.csv"

static int	_displayPrices(t_file inputvalues)
{
	BitcoinExchange be(BE_DBRATES, inputvalues);

	return (be.displayPrices());
}

static void	_help(char **av)
{
	std::cout << av[0] << ": info: please the program as such:" << std::endl;
	std::cout << av[0] << " [text_file]" << std::endl;
}

int	main(int ac, char **av)
{
	if (ac != 2)
	{
		_help(av);
		return (2);
	}
	return (_displayPrices(av[1]));
}