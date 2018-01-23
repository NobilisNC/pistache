/*
 * Pistache -- C++ templated string formatting library
 * <https://github.com/NobilisNC/pistache>
 *
 * Copyright (c) 2018 Tom Henoch
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */
#ifndef _PISTACHE_SINGLE_HEADER_H_
#define _PISTACHE_SINGLE_HEADER_H_

#include <string>
#include <map>
#include <vector>

namespace pistache {

using Context = std::map<std::string, std::string>;

class String
{
 public :
    explicit String(const std::string& tmpl = "");
    ~String() = default;

    void setTemplate(const std::string& tmpl);

    std::string get(const Context& ctx) const;
    std::string operator()(const Context& ctx) const;

    std::vector<std::string> placeholders() const;
    std::vector<std::string> fragments() const;

 private :
    struct Part
    {
     bool is_placeholder;
     std::string value;
    };
    void parse(const std::string& tmpl);
    Part& savePart(const std::string& tmpl, std::size_t begin,
                   std::size_t part_length, bool is_placeholder);

 private :
    std::vector<Part> parts_;
};

#ifdef PISTACHE_IMPL

String::String(const std::string& tmpl)
{
  parse(tmpl);
}

void String::setTemplate(const std::string& tmpl)
{
  parts_.clear();
  parse(tmpl);
}

std::vector<std::string> String::placeholders() const
{
  std::vector<std::string> placeholders;
  for (const Part& p : parts_)
    if (p.is_placeholder)
      placeholders.push_back(p.value);

    return placeholders;
}

std::vector<std::string> String::fragments() const
{
  std::vector<std::string> fragments;
  for (const Part& p : parts_)
    if (!p.is_placeholder)
      fragments.push_back(p.value);

    return fragments;
}

std::string String::get(const Context& ctx) const
{
  std::string ret;

  for (const Part& p : parts_) {
      if (p.is_placeholder) {
        // Find the value to avoid exceptions instead of .at(key) or [key]
        auto found = ctx.find(p.value);
        if (found != ctx.end())
          ret += found->second;
      } else {
        ret += p.value;
      }
  }

  return ret;
}

std::string String::operator()(const Context& ctx) const
{
  return get(ctx);
}

void String::parse(const std::string& tmpl)
{
  constexpr char placeholder_begin = '{';
  constexpr char placeholder_end   = '}';

  bool is_placeholder = false;
  std::size_t begin_part = 0;
  std::size_t part_length = -1;

  for (std::size_t i = 0; i < tmpl.length(); ++i) {
    // when part_length equal 0, we begin a new part of the String
    if (++part_length == 0)
      begin_part = i;

    // if the end is reached we save the part
    if (!is_placeholder && i == tmpl.length() - 1)
      savePart(tmpl, begin_part, part_length + 1, is_placeholder);

    // if we found a new placehoder or we reached the end of placeholder
    if (!is_placeholder && tmpl[i] == placeholder_begin
        || is_placeholder && tmpl[i] == placeholder_end ) {
      // we save the last part
      savePart(tmpl, begin_part, part_length, is_placeholder);
      // We begin a new part
      is_placeholder = !is_placeholder;
      part_length = -1;
    }  // else we do nothing
  }
}

String::Part& String::savePart(const std::string& tmpl, std::size_t begin,
                       std::size_t part_length, bool is_placeholder) {

parts_.emplace_back(Part{ is_placeholder, tmpl.substr(begin, part_length) });
return parts_.back();

}

#endif  // PISTACHE_IMPL

}  // namespace pistache

#endif // _PISTACHE_SINGLE_HEADER_H_
