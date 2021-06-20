/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FileUpload.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aduregon <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/15 15:03:15 by aduregon          #+#    #+#             */
/*   Updated: 2021/06/18 14:44:35 by aduregon         ###   ########.fr       */
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
		long int time = std::chrono::duration_cast<std::chrono::milliseconds>(p1.time_since_epoch()).count();

		if (req.filename.compare(""))
			name = req.filename; //+ "_" + std::to_string(time);
		else
			name = "file_" + std::to_string(time);
		//std::ifstream file("./upload/" + name);
		std::ofstream file("./upload/" + name);
		file << req.body;
		file.close();
	}

	~FileUpload() {}
};
