/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FileUpload.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aduregon <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/15 15:03:15 by aduregon          #+#    #+#             */
/*   Updated: 2021/06/18 10:12:21 by aduregon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <fstream>
#include <chrono>
#include <sstream>

class FileUpload
{
private:
	/* data */
public:
	FileUpload(/* args */) {}

	FileUpload(Request req)
	{
		std::string name;
		std::chrono::time_point<std::chrono::system_clock> p1 = std::chrono::system_clock::now();
		if (!(req.transfer_encoding.compare(0, 7, "chunked")))
		{
			long int time = std::chrono::duration_cast<std::chrono::milliseconds>(p1.time_since_epoch()).count();

			if (req.filename.compare(""))
				name = req.filename; //+ "_" + std::to_string(time);
			else
				name = "file_" + std::to_string(time);
			std::ofstream file("./upload/" + name);
			int	size = 0;
			for (size_t i = 0; i < req.body.length(); i++)
			{
				std::string tmp;
				// 13 == \r
				while (req.body[i] != 13)
				{
					tmp += req.body[i];
					i++;
				}
				size = htoi(tmp);
				if (!size)
					break ;
				i += 2 ;
				for (size_t j = 0; j < (size_t)size; j++)
				{
					file << req.body[i];
					i++;
				}
			}
			file.close();
		}
		else
		{
			long int time = std::chrono::duration_cast<std::chrono::milliseconds>(p1.time_since_epoch()).count();

			if (req.filename.compare(""))
				name = req.filename; //+ "_" + std::to_string(time);
			else
				name = "file_" + std::to_string(time);
			std::ifstream file("./upload/" + name);
			if (!file.is_open())
			{
				std::ofstream file("./upload/" + name);
				file << req.body;
			}
			file.close();
		}
	}

	int		htoi(std::string hex)
	{
		std::stringstream x;
		int res;
		
		x << std::hex << hex;
		x >> res;
		return res;
	}

	~FileUpload() {}
};
