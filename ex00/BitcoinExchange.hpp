/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BitcoinExchange.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jerperez <jerperez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/03 10:49:08 by jerperez          #+#    #+#             */
/*   Updated: 2024/05/03 16:47:54 by jerperez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BITCOINEXCHANGE_HPP
# define BITCOINEXCHANGE_HPP

# include <string>
# include <time.h>
# include <fstream>
# include <vector>
# include <iostream>
typedef	std::string	t_file;

class BitcoinExchange
{
	private:
		class	CSVLine
		{
			private:
				class	InvalidDateException : public std::exception
				{
					public:
						const char	*what(void) const throw();
				};
				class	InvalidValueException : public std::exception
				{
					public:
						const char	*what(void) const throw();
				};
				class	InvalidLineException : public std::exception
				{
					public:
						const char	*what(void) const throw();
				};
				int					_value_int;
				float				_value_float;
				struct tm			_time;
			public:
				CSVLine(void);
				~CSVLine(void);
				CSVLine(const CSVLine&);
				CSVLine(std::string db, std::string intput);
				CSVLine	&operator=(const CSVLine&);
				CSVLine	operator*(const CSVLine&);
				struct	datecomp
				{
					bool operator()(const CSVLine&, const CSVLine&) const;
				};
				bool		operator<(int);
				bool		operator>(int);
				bool		parseDate(std::string);
				bool		parseRate(std::string);
				std::string	getDate(void) const;
				std::string	getValue(void) const;
		};
		BitcoinExchange(void);
		t_file					_dbrates;
		std::vector<CSVLine>	_csv;
		t_file					_intputvalues;
		bool					_parseInput(void);
		std::vector<CSVLine>	_parseCSV(std::string);
		void					_displayInfo(CSVLine);
		void					_error(std::string);
	public:
		BitcoinExchange(t_file, t_file);
		~BitcoinExchange(void);
		BitcoinExchange(const BitcoinExchange&);
		BitcoinExchange	&operator=(const BitcoinExchange&);
		bool	displayPrices(void);
};

#endif
