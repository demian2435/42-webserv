/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FileUpload.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmalori <dmalori@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/15 15:03:15 by aduregon          #+#    #+#             */
/*   Updated: 2021/06/17 11:04:03 by dmalori          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <fstream>
#include <chrono>

class FileUpload
{
private:
	/* data */
public:
	FileUpload(/* args */) {}

	FileUpload(std::string body)
	{
		std::string name;
		std::chrono::time_point<std::chrono::system_clock> p1 = std::chrono::system_clock::now();
 
		while (1)
		{
			long int time = std::chrono::duration_cast<std::chrono::milliseconds>(p1.time_since_epoch()).count();
			name = "file_" + std::to_string(time);
			std::ifstream file("./upload/" + name);

			if (!file.is_open())
			{
				std::ofstream file("./upload/" + name);
				file << body;
				break ;
			}
			else
				continue ;
		}
	}

	~FileUpload() {}
};
