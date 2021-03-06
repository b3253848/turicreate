/* Copyright © 2017 Apple Inc. All rights reserved.
 *
 * Use of this source code is governed by a BSD-3-clause license that can
 * be found in the LICENSE.txt file or at https://opensource.org/licenses/BSD-3-Clause
 */
#ifndef TURI_SFRAME_TESTING_UTILS_H_
#define TURI_SFRAME_TESTING_UTILS_H_

#include <sframe/sframe.hpp>
#include <parallel/pthread_tools.hpp>
#include <vector>
#include <string>
#include <locale>

namespace turi {

sframe make_testing_sframe(const std::vector<std::string>& names,
                           const std::vector<flex_type_enum>& types,
                           const std::vector<std::vector<flexible_type> >& data);

sframe make_testing_sframe(const std::vector<std::string>& names,
                           const std::vector<std::vector<flexible_type> >& data);

sframe make_integer_testing_sframe(const std::vector<std::string>& names,
                                   const std::vector<std::vector<size_t> >& data);

std::vector<std::vector<flexible_type> > testing_extract_sframe_data(const sframe& sf);


std::shared_ptr<sarray<flexible_type> >
make_testing_sarray(flex_type_enum types,
                    const std::vector<flexible_type>& data);



/**
 * \internal
 * \ingroup sframe_physical
 * \addtogroup sframe_internal SFrame Internal
 * 
 *  Creates a random SFrame for testing purposes.  The
 *  column_type_info gives the types of the column.
 *
 *  \param[in] n_rows The number of observations to run the timing on.
 *  \param[in] column_type_info A string with each character denoting
 *  one type of column.  The legend is as follows:
 *
 *     n:  numeric column.
 *     b:  categorical column with 2 categories.
 *     z:  categorical column with 5 categories.
 *     Z:  categorical column with 10 categories.
 *     c:  categorical column with 100 categories.
 *     C:  categorical column with 1000000 categories.
 *     s:  categorical column with short string keys and 1000 categories.
 *     S:  categorical column with short string keys and 100000 categories.
 *     v:  numeric vector with 10 elements.
 *     V:  numeric vector with 1000 elements.
 *     u:  categorical set with up to 10 elements.
 *     U:  categorical set with up to 1000 elements.
 *     d:  dictionary with 10 entries.
 *     D:  dictionary with 100 entries.
 *
 *  \param[in] create_target_column If true, then create a random
 *  target column called "target" as well.
*/
sframe make_random_sframe(size_t n_rows, std::string column_types, bool create_target_column = false);

////////////////////////////////////////////////////////////////////////////////

template <typename T, typename U>
std::vector<T> testing_extract_column(std::shared_ptr<sarray<U> > col) {

  auto reader = col->get_reader();

  size_t num_segments = col->num_segments();

  std::vector<T> ret;
  ret.reserve(col->size());

  for(size_t sidx = 0; sidx < num_segments; ++sidx) {
    auto src_it     = reader->begin(sidx);
    auto src_it_end = reader->end(sidx);

    for(; src_it != src_it_end; ++src_it)
      ret.push_back(T(*src_it));
  }

  return ret;
}

template <typename T>
std::vector<T> testing_extract_column_non_flex(std::shared_ptr<sarray<T> > col) {

  auto reader = col->get_reader();

  size_t num_segments = col->num_segments();

  std::vector<T> ret;
  ret.reserve(col->size());

  for(size_t sidx = 0; sidx < num_segments; ++sidx) {
    auto src_it     = reader->begin(sidx);
    auto src_it_end = reader->end(sidx);

    for(; src_it != src_it_end; ++src_it)
      ret.push_back(T(*src_it));
  }

  return ret;
}

// Turn a vector into an sarray (non flexible_type version).
template <typename T>
std::shared_ptr<sarray<T> > make_testing_sarray(const std::vector<T>& col) {
  std::shared_ptr<sarray<T> > new_x(new sarray<T>);

  new_x->open_for_write(1);
  auto it_out = new_x->get_output_iterator(0);

  for(const auto& row : col) {
    *it_out = row;
    ++it_out;
  }

  new_x->close();

  return new_x;
}

////////////////////////////////////////////////////////////////////////////////

sframe slice_sframe(const sframe& src, size_t row_lb, size_t row_ub);

}

#endif /* _TESTING_UTILS_H_ */
