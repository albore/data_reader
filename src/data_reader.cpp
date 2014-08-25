/** Class to calculate marginals and several other stuff from a Factor Graph.
 *  Copyright (c) 2014 A. Albore.
 *  Use of this source code is governed by a BSD-style license as follows:
 @author Alexandre Albore
 @version 1.0
 * Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

* 1. Redistributions of source code must retain the above copyright notice, this
  list of conditions and the following disclaimer.
* 2. Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
**/



#include "ros/ros.h"
#include <data_reader/DataReader.h>
#include <fstream>

/** 
    This data_reader ros package offers a 'service' that delivers the content of a vector
    containing data, provided that the client node sends the row and column of the
    desired data information.
*/

std::vector<unsigned> data;

/**
   Prints command line usage.
   @param exec the command
*/
void usage(char *exec)
{
        std::cout << "Usage: " << exec << " <filename>"  << std::endl; 
        std::cout << "   <file>: file containing the data." << std::endl;
        std::cout << std::endl;
        exit(0);
}

/**
   Stores the data in a vector.
   @param filename Text file containing the data.
*/
void read_input_file(const char* filename)
{
        std::string current_line;
        std::ifstream infile;
        try {
                infile.open (filename);
        } catch (int e) {
                std::cout << "An exception occurred. Cannot read from file." << std::endl; 
                ROS_FATAL("Input file is missing!");
                exit(0);
        }
        unsigned input;
        char next;
        size_t n=0;
        //data.push_back(std::vector<unsigned>());
        while (!infile.eof())
        {
                infile >> input;
                // data[n].push_back(input);
                data.push_back(input);
                infile.get(next);
                if(next==' ' || next == '\t')
                        continue;
                else if(next=='\n'){
                        ++n;
                        //   data.push_back(std::vector<unsigned>());
                }  
        }
	infile.close();
//	system ("pause");
        /* Print data */
        std::cout << "Gathered data: " << std::endl;
        // for(size_t i = 0; i<data.size(); i++)
        //         for(size_t j = 0; j<data[i].size(); j++) {
        //                 std::cout << data[i][j];
        //                 if (j+1 == data[i].size())
        //                         std::cout << std::endl;
        //         }
}



bool get_data(data_reader::DataReader::Request  &req,
                  data_reader::DataReader::Response &res)
{
        try {
                res.num = data[req.x];
//res.num = data[req.coord.x][req.coord.y];
        } catch ( const int exception[] )   {
                std::cout << exception
                          << "Array Out Of Bounds"
                          <<  std::endl;
                return 0;
        }
        ROS_INFO("request: id=%ld", (long int)req.x);
//        ROS_INFO("request: x=%ld, y=%ld", (long int)req.coord.x, (long int)req.coord.y);
        ROS_INFO("sending back response: [%ld]", (long int)res.num);
        return true;
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "data_reader_server");
  ros::NodeHandle n;

  ROS_INFO("started with: %s %s",argv[0], argv[1]);
  if (argc > 1)
          read_input_file(argv[1]);
  else  usage(argv[0]);


  ros::ServiceServer service = n.advertiseService(std::string("data_reader"), get_data);
  ROS_INFO("Ready to send back data.");
  ros::spin();

  return 0;
}
