/*********************************************************************************
*     File Name           :     https://github.com/AndsonYe/MultipartEncoder/blob/master/multipart_parser.h
*     Created By          :     Ye Yangang
*     Creation Date       :     [2017-02-20 16:50]
*     Last Modified       :     [AUTO_UPDATE_BEFORE_SAVE]
*     Description         :     Generate multipart/form-data POST body
**********************************************************************************/

/*
MIT License

Copyright (c) 2020 YangangYe

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef GEN_MULTIPART_H_
#define GEN_MULTIPART_H_

#include <vector>
#include <string>
#include <tuple>

namespace web{
namespace http{

class MultipartParser
{
public:
  MultipartParser();
  inline const std::string &body_content()
  {
    return body_content_;
  }
  inline const std::string &boundary()
  {
    return boundary_;
  }
  inline void AddParameter(const std::string &name, const std::string &value)
  {
    params_.push_back(std::move(std::pair<std::string, std::string>(name, value)));
  }
  inline void AddFile(const std::string &name, const std::string &value)
  {
    files_.push_back(std::move(std::pair<std::string, std::string>(name, value)));
  }
  const std::string &GenBodyContent();

private:
  void _get_file_name_type(const std::string &file_path, std::string *filenae, std::string *content_type);
private:
  static const std::string boundary_prefix_;
  static const std::string rand_chars_;
  std::string boundary_;
  std::string body_content_;
  std::vector<std::pair<std::string, std::string> > params_;
  std::vector<std::pair<std::string, std::string> > files_;
};

} //namespace web::http
} //namespace web

#endif
