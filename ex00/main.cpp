/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jerperez <jerperez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/03 10:41:02 by jerperez          #+#    #+#             */
/*   Updated: 2024/05/03 11:23:48 by jerperez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include "BitcoinExchange.hpp"

#define BE_DBRATES "data.csv"

static int	_displayPrices(t_file inputvalues)
{
	BitcoinExchange be(BE_DBRATES, inputvalues);

	return (be.displayPrices());
}

int	main(int ac, char **av)
{
	if (ac != 2)
		return (1);
	return (_displayPrices(av[1]));
}