/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BitcoinExchange.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jerperez <jerperez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/03 10:49:08 by jerperez          #+#    #+#             */
/*   Updated: 2024/05/03 11:23:09 by jerperez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BITCOINEXCHANGE_HPP
# define BITCOINEXCHANGE_HPP

# include <string>
typedef	std::string	t_file;

class BitcoinExchange
{
	private:
		typedef	struct		_t_date;
		typedef	struct		_t_scalar;
		BitcoinExchange(void);
		t_file				_dbrates;
		t_file				_intputvalues;
		_t_date				_parseDate(std::string);
		_t_scalar			_parseValue(std::string);
		_t_scalar			_parseRate(std::string);
		_t_scalar			_getPrice(_t_scalar, _t_scalar);
		void				_error(std::string);
	public:
		BitcoinExchange(t_file, t_file);
		~BitcoinExchange(void);
		BitcoinExchange(const BitcoinExchange&);
		BitcoinExchange	&operator=(const BitcoinExchange&);
		bool	displayPrices(void);
};

#endif
